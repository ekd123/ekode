#include <bson-glib/bson-glib.h>

int main ()
{
    GBsonIter iter;
    GBson *bson = g_bson_new_empty ();
    g_bson_append_string (bson, "key", "value");
    g_bson_append_int64 (bson, "int64", 12345);
    g_bson_iter_init (&iter, bson);
    while (g_bson_iter_next (&iter))
    {
        GBsonType type = g_bson_iter_get_value_type (&iter);
        switch (type)
        {
        case G_BSON_INT64:
            g_print ("%s : %ld\n", g_bson_iter_get_key (&iter), g_bson_iter_get_value_int64 (&iter));
            break;
        case G_BSON_UTF8:
            g_print ("%s : %s\n", g_bson_iter_get_key (&iter), g_bson_iter_get_value_string (&iter, NULL));
            break;
        default:
            g_print ("what's this? %s\n", g_bson_iter_get_key (&iter));
            break;
        }
    }
    g_bson_unref (bson);
    return 0;
}

