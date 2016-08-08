#ifndef DEMO_APPWINDOW_H
#define DEMO_APPWINDOW_H

#include <gtk/gtk.h>

#include "demo-app.h"

#define DEMO_TYPE_APP_WINDOW (demo_app_window_get_type ())
#define DEMO_APP_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),DEMO_TYPE_APP_WINDOW, DemoAppWindow))

typedef struct _DemoAppWindow DemoAppWindow;
typedef struct _DemoAppWindowClass DemoAppWindowClass;

G_BEGIN_DECLS

GType          demo_app_window_get_type (void);
DemoAppWindow *demo_app_window_new      (DemoApp *app);
void           demo_app_window_open     (DemoAppWindow *win,
                                         GFile         *file);

G_END_DECLS

#endif /* DEMO_APPWINDOW_H */
