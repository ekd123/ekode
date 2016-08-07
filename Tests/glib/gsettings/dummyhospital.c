#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>
#include "dummyhospital.h"

/* 私有信息 */
struct _DummyHospitalPrivate
{
    GString    *patients;
    GSettings  *settings;
};

/* 类的实现 */
G_DEFINE_TYPE(DummyHospital, dummy_hospital, G_TYPE_OBJECT)

static void
dummy_hospital_dispose (GObject *gobject)
{
    DummyHospital *self = DUMMY_HOSPITAL (gobject);
    
    if (self->priv->settings)
    {
        g_object_unref (self->priv->settings);
        self->priv->settings = NULL;
    }
    
    G_OBJECT_CLASS (dummy_hospital_parent_class)->dispose (gobject);
    
    g_debug ("disposing!");
}

static void
dummy_hospital_finalize (GObject *gobject)
{
    DummyHospital *self = DUMMY_HOSPITAL (gobject);
    
    if (self->priv->patients)
    {
        g_string_free (self->priv->patients, TRUE);
        self->priv->patients = NULL;
    }
    
    G_OBJECT_CLASS (dummy_hospital_parent_class)->finalize (gobject);
    g_debug ("the gate is closed!");
}

static void
dummy_hospital_class_init (DummyHospitalClass *klass)
{
    g_type_class_add_private (klass, sizeof (DummyHospitalPrivate));
    
    GObjectClass *base_class = G_OBJECT_CLASS (klass);
    base_class->dispose      = dummy_hospital_dispose;
    base_class->finalize     = dummy_hospital_finalize;
}

static void 
dummy_hospital_init (DummyHospital *self)
{
    self->priv = DUMMY_GET_HOSPITAL_PRIVATE (self);
    self->priv->settings = g_settings_new ("org.Example.DummyHospital");
    
    char *t = g_settings_get_string (self->priv->settings,
                                     "patients");
    self->priv->patients = g_string_new (t);
    g_free (t);
    
    g_debug ("the gate is open!");
}


void
dummy_hospital_sync (DummyHospital *self)
{
    g_settings_set_string (self->priv->settings, "patients", 
                           self->priv->patients->str);
}

gboolean 
dummy_hospital_look_for (DummyHospital *self,
                         const gchar *who)
{
    /* 这里的搜寻并不准确，因为会有这样的现象：
     * 一个人名为 AB，另一个是 B，那么 B 就会出现两次
     * 更好的办法是读取每一行并进行 strcmp。
     */
    if (g_strstr_len (self->priv->patients->str, -1, who))
        return TRUE;
    return FALSE;
}

gboolean
dummy_hospital_be_in_hospital (DummyHospital *self, 
                               const gchar *who)
{
    if (dummy_hospital_look_for (self, who))
    {
        g_error ("Sorry. Some issues cause us can't make"
                 " you be in our hospital, because your "
                 "name is same as another one :-(");
        return FALSE;
    }
    
    g_string_append (self->priv->patients, who);
    
    dummy_hospital_sync (self);
    
    g_debug ("Patinent %s is be in our hospital.", who);
    return TRUE;
}

gboolean
dummy_hospital_leave_hospital (DummyHospital *self,
                               const gchar *who)
{
    g_return_val_if_fail ((DUMMY_IS_HOSPITAL (self) != FALSE), FALSE);
    g_return_val_if_fail (who != NULL, FALSE);
    
    if (dummy_hospital_look_for (self, who) == FALSE)
    {
        g_error ("404 Not found.");
        return FALSE;
    }
    
    GString *lookfor = g_string_new ("");
    g_string_append_printf (lookfor, "%s\n", who);
    gchar *ptr = g_strstr_len (self->priv->patients->str, -1, lookfor->str);
    gint pos = ptr - self->priv->patients->str;
    g_string_erase (self->priv->patients, pos, lookfor->len);
    g_string_free (lookfor, TRUE);
    
    dummy_hospital_sync (self);
    
    g_debug ("Patient %s left our hospital.", who);
    return TRUE;
}

gboolean 
dummy_hospital_print_list (DummyHospital *self)
{
    gchar **arr = g_strsplit (self->priv->patients->str, "\n", 0);
    guint len   = g_strv_length (arr) - 1;/* 总会多一个换行符 */
    
    g_print ("There are %d patients\n", len);
    int i;
    for (i=0; i<len; i++)
    {
        g_print ("* %s\n", arr[i]);
    }
    g_strfreev (arr);
    return TRUE;
}

