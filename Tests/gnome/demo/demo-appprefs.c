#include <gtk/gtk.h>

#include "demo-appprefs.h"

struct _DemoAppPrefs
{
    GtkDialog parent;
};
struct _DemoAppPrefsClass
{
    GtkDialogClass parent_class;
};
typedef struct _DemoAppPrefsPrivate DemoAppPrefsPrivate;
struct _DemoAppPrefsPrivate
{
    GSettings *settings;
    GtkWidget *font;
    GtkWidget *transition;
};

static void demo_app_prefs_init (DemoAppPrefs *prefs);

G_DEFINE_TYPE_WITH_PRIVATE(DemoAppPrefs, demo_app_prefs, GTK_TYPE_DIALOG);

static void
demo_app_prefs_init (DemoAppPrefs *prefs)
{
    DemoAppPrefsPrivate *priv;
    
    priv = demo_app_prefs_get_instance_private (prefs);
    gtk_widget_init_template (GTK_WIDGET (prefs));
    priv->settings = g_settings_new ("org.gnome.demoapp");
    
    g_settings_bind (priv->settings, "font", 
                     priv->font, "font",
                     G_SETTINGS_BIND_DEFAULT);
    g_settings_bind (priv->settings, "transition",
                     priv->transition, "active-id",
                     G_SETTINGS_BIND_DEFAULT);
}

static void
demo_app_prefs_dispose (GObject *object)
{
    DemoAppPrefsPrivate *priv;
    
    priv = demo_app_prefs_get_instance_private (DEMO_APP_PREFS (object));
    g_clear_object (&priv->settings);
    
    G_OBJECT_CLASS (demo_app_prefs_parent_class)->dispose (object);
}

static void
demo_app_prefs_class_init (DemoAppPrefsClass *class)
{
    G_OBJECT_CLASS (class)->dispose = demo_app_prefs_dispose;
    
    gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class), 
                                                 "/org/gnome/demoapp/prefs.ui");
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), 
                                                  DemoAppPrefs, font);
    gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class), 
                                                  DemoAppPrefs, transition);
}

DemoAppPrefs *
demo_app_prefs_new (DemoAppWindow *win)
{
    return g_object_new (DEMO_TYPE_APP_PREFS, "transient-for", win, 
                         "use-header-bar", TRUE, NULL);
}
