#include <gtk/gtk.h>
#include "demo-app.h"

gint 
main (gint   argc,
      gchar *argv[])
{
    return g_application_run (G_APPLICATION (demo_app_new ()), argc, argv);
}
