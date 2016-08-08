#include "demo-app.h"
#include "demo-appwindow.h"
#include "demo-appprefs.h"

static void demo_app_init (DemoApp *app);

struct _DemoApp
{
    GtkApplication parent;
};

struct _DemoAppClass
{
    GtkApplicationClass parent_class;
};

G_DEFINE_TYPE(DemoApp, demo_app, GTK_TYPE_APPLICATION);

static void
demo_app_init (DemoApp *app)
{
}

static void
demo_app_activate (GApplication *app)
{
    DemoAppWindow *win;
    
    win = demo_app_window_new (DEMO_APP (app));
    gtk_window_present (GTK_WINDOW (win));
}

static void
demo_app_open (GApplication  *app,
               GFile        **files,
               gint           n_files,
               const gchar   *hint)
{
    GList         *windows;
    DemoAppWindow *win;
    gint           i;
    
    windows = gtk_application_get_windows (GTK_APPLICATION (app));
    if (windows)
        win = DEMO_APP_WINDOW (windows->data);
    else
        win = demo_app_window_new (DEMO_APP (app));
    
    for (i = 0; i < n_files; i++)
        demo_app_window_open (win, files[i]);
    
    gtk_window_present (GTK_WINDOW (win));
}

static void
preferences_activated (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       app)
{
    DemoAppPrefs *prefs;
    GtkWindow    *win;
    
    win = gtk_application_get_active_window (GTK_APPLICATION (app));
    prefs = demo_app_prefs_new (DEMO_APP_WINDOW (win));
    gtk_window_present (GTK_WINDOW (prefs));
}

static void
quit_activated (GSimpleAction *action,
                GVariant      *parameter,
                gpointer       app)
{
    g_application_quit (G_APPLICATION (app));
}

static GActionEntry app_entries[] = 
{
    { "preferences", preferences_activated, NULL, NULL, NULL },
    { "quit", quit_activated, NULL, NULL, NULL }
};

static void
demo_app_startup (GApplication *app)
{
    GtkBuilder *builder;
    GMenuModel *app_menu;
    const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };
    G_APPLICATION_CLASS (demo_app_parent_class)->startup (app);
    g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries, 
        G_N_ELEMENTS (app_entries), app);
    gtk_application_set_accels_for_action (GTK_APPLICATION (app), "app.quit", quit_accels);
    builder = gtk_builder_new_from_resource ("/org/gnome/demoapp/app-menu.ui");
    app_menu = G_MENU_MODEL (gtk_builder_get_object (builder, "appmenu"));
    gtk_application_set_app_menu (GTK_APPLICATION (app), app_menu);
    g_object_unref (builder);
}

static void
demo_app_class_init (DemoAppClass *class)
{
    G_APPLICATION_CLASS (class)->activate = demo_app_activate;
    G_APPLICATION_CLASS (class)->open     = demo_app_open;
    G_APPLICATION_CLASS (class)->startup  = demo_app_startup;
}

DemoApp *
demo_app_new (void)
{
    return g_object_new (DEMO_TYPE_APP,
                         "application-id", "org.gnome.demoapp",
                         "flags", G_APPLICATION_HANDLES_OPEN,
                         NULL);
}

