#ifndef DEMO_APP_H
#define DEMO_APP_H

#include <gtk/gtk.h>

#define DEMO_TYPE_APP (demo_app_get_type ())
#define DEMO_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj),DEMO_TYPE_APP, DemoApp))

typedef struct _DemoApp DemoApp;
typedef struct _DemoAppClass DemoAppClass;

G_BEGIN_DECLS

GType    demo_app_get_type (void);
DemoApp *demo_app_new      (void);

G_END_DECLS

#endif /* DEMO_APP_H */
