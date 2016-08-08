#ifndef DEMO_APPPREFS_H
#define DEMO_APPPREFS_H

#include <gtk/gtk.h>

#include "demo-appwindow.h"

#define DEMO_TYPE_APP_PREFS (demo_app_prefs_get_type ())
#define DEMO_APP_PREFS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),DEMO_TYPE_APP_PREFS,DemoAppPrefs))

typedef struct _DemoAppPrefs DemoAppPrefs;
typedef struct _DemoAppPrefsClass DemoAppPrefsClass;

G_BEGIN_DECLS

GType         demo_app_prefs_get_type (void);
DemoAppPrefs *demo_app_prefs_new      (DemoAppWindow *app);

G_END_DECLS

#endif /* DEMO_APPPREFS_H */
