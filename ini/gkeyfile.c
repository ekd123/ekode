#include <glib.h>

int main()
{
    GKeyFile *kf;
    gchar **keys;
    gsize length;
    kf = g_key_file_new();
    g_key_file_load_from_file(kf, "test.ini", G_KEY_FILE_NONE, NULL);
    keys = g_key_file_get_keys(kf, "Section1000", &length, NULL);
    for(int i = 0; i < length; i++)
    {
        gchar *val;
        val = g_key_file_get_value(kf, "Section1000", keys[i], NULL);
        g_printf("%s = %s\n", keys[i], val);
        g_free(val);
    }
    g_key_file_unref(kf);
    g_strfreev(keys);
    return 0;
}
