#include <gtk/gtk.h>

#include "demo-app.h"
#include "demo-appwindow.h"

struct _DemoAppWindow
{
    GtkApplicationWindow parent;
};
struct _DemoAppWindowClass
{
    GtkApplicationWindowClass parent_class;
};

typedef struct _DemoAppWindowPrivate DemoAppWindowPrivate;
struct _DemoAppWindowPrivate
{
    GtkWidget *stack;
    GSettings *settings;
};

static void demo_app_window_init (DemoAppWindow *win);

G_DEFINE_TYPE_WITH_PRIVATE(DemoAppWindow, demo_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
demo_app_window_init (DemoAppWindow *win)
{
    DemoAppWindowPrivate *priv;
    
    priv = demo_app_window_get_instance_private (win);
    gtk_widget_init_template (GTK_WIDGET (win));
    priv->settings = g_settings_new ("org.gnome.demoapp");
    
    g_settings_bind (priv->settings, "transition", 
                     priv->stack, "transition-type",
                     G_SETTINGS_BIND_DEFAULT);
}

static void
demo_app_window_class_init (DemoAppWindowClass *class)
{
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                                 "/org/gnome/demoapp/window.ui");
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class),
                                                  DemoAppWindow, stack);
}

DemoAppWindow *
demo_app_window_new (DemoApp *app)
{
  return g_object_new (DEMO_TYPE_APP_WINDOW, "application", app, NULL);
}

void
demo_app_window_open (DemoAppWindow *win,
                      GFile         *file)
{
    DemoAppWindowPrivate *priv;
    gchar                *basename;
    GtkWidget            *scrolled,
                         *view;
    gchar                *contents;
    gsize                 length;
    
    priv = demo_app_window_get_instance_private (win);
    basename = g_file_get_basename (file);
    
    scrolled = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_hexpand (scrolled, TRUE);
    gtk_widget_set_vexpand (scrolled, TRUE);
    view = gtk_text_view_new ();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), FALSE);
    gtk_container_add (GTK_CONTAINER (scrolled), view);
    gtk_stack_add_titled (GTK_STACK (priv->stack), scrolled, basename, basename);
    if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
    {
        GtkTextBuffer *buffer;
        
        buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
        gtk_text_buffer_set_text (buffer, contents, length);
        g_free (contents);
    }
    
    gtk_widget_show(scrolled);
    gtk_widget_show(view);
    
    g_free (basename);
}

