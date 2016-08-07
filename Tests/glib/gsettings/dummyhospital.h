#ifndef DUMMY_HOSPITAL_H
#define DUMMY_HOSPITAL_H

#include <glib-object.h>

G_BEGIN_DECLS

#define DUMMY_TYPE_HOSPITAL            (dummy_hospital_get_type ())
#define DUMMY_HOSPITAL(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), DUMMY_TYPE_HOSPITAL, DummyHospital))
#define DUMMY_HOSPITAL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), DUMMY_TYPE_HOSPITAL, DummyHospitalClass))
#define DUMMY_IS_HOSPITAL(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), DUMMY_TYPE_HOSPITAL))
#define DUMMY_IS_HOSPITAL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DUMMY_TYPE_HOSPITAL))
#define DUMMY_GET_HOSPITAL_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), DUMMY_TYPE_HOSPITAL, DummyHospitalClass))
#define DUMMY_GET_HOSPITAL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), DUMMY_TYPE_HOSPITAL, DummyHospitalPrivate))

typedef struct _DummyHospital         DummyHospital;
typedef struct _DummyHospitalClass    DummyHospitalClass;
typedef struct _DummyHospitalPrivate  DummyHospitalPrivate;

struct _DummyHospital
{
    GObject parent;
    
    /*< Private >*/
    DummyHospitalPrivate *priv;
};

struct _DummyHospitalClass
{
    GObjectClass parent_class;
};

G_END_DECLS

GType    dummy_hospital_get_type ();
void dummy_hospital_sync (DummyHospital *self);
gboolean dummy_hospital_look_for (DummyHospital *self, const gchar *who);
gboolean dummy_hospital_leave_hospital (DummyHospital *self,const gchar *who);
gboolean dummy_hospital_be_in_hospital (DummyHospital *self, const gchar *who);
gboolean dummy_hospital_print_list (DummyHospital *self);
#endif
