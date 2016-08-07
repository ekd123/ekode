#include <glib.h>
/*
  this is an example shows how to use GMarkup.

  The example XML format is 
  <data>
    <entry name="">data here</entry>
  </data>

  a file is allowed to have only one <data>.

  For a complete tut for GMarkup, please visit http://blogs.gnome.org/tthurman/2008/02/14/gmarkup/
 */

gboolean got_data = FALSE;
gboolean got_entry = FALSE;
gboolean check_only = FALSE;

gchar *entry_name = NULL;

void start_element (GMarkupParseContext *context,
                    const gchar         *element_name,
                    const gchar        **attribute_names,
                    const gchar        **attribute_values,
                    gpointer             user_data,
                    GError             **error)
{
    if (!g_strcmp0 (element_name, "data"))
    {
        if (got_data)
            g_critical ("A file is allowed to have only one <data>");
        if (!got_data)
            got_data = TRUE;
    }
    if (!g_strcmp0 (element_name, "entry"))
    {
        /* inside a <data>? */
        if (!got_data)
            g_critical ("<data> must be the first element");
        /* entry occurred */
        if (got_entry)
            g_critical ("There can't be an <entry> inside an <entry>");
        if (!got_entry)
            got_entry = TRUE;

        if (g_strv_length (attribute_names) != 1 || 
            g_strv_length (attribute_names) != g_strv_length (attribute_values))
            g_critical ("<entry> must have only one attribute called name");
        entry_name = g_strdup (*attribute_values);
    }
}

void 
text (GMarkupParseContext *context,
      const gchar         *text,
      gsize                text_len,
      gpointer             user_data,
      GError             **error)
{
    if (entry_name && !check_only)
        g_print ("Got an entry called %s with data %*s\n",
                 entry_name, text_len, text);
}

void 
end_element (GMarkupParseContext *context,
             const gchar         *element_name,
             gpointer             user_data,
             GError             **error)
{
    if (!g_strcmp0 (element_name, "entry"))
    {
        if (entry_name)
        {
            g_free (entry_name);
            entry_name = NULL;
        }
        got_entry = FALSE;
    }
}

void
error (GMarkupParseContext *context,
       GError              *error,
       gpointer             user_data)
{
    g_critical ("an unexpected error occurred: %s", error->message);
}

int main (int argc, char *argv[])
{
    GMarkupParser parser = {
        start_element,
        end_element,
        text,
        NULL,
        error
    };

    gchar *filename;
    gchar *text; gsize length;
    GMarkupParseContext *context = g_markup_parse_context_new (
        &parser,
        0,
        NULL,
        NULL);

    switch (argc)
    {
    case 3:
        check_only = TRUE;
    case 2:
        filename = argv[1];
        break;
    default:
        g_print ("./a.out FILENAME (CHECK_ONLY)\n");
        return -1;
        break;
    }

    if (g_file_get_contents (filename, &text, &length, NULL) ==FALSE)
    {
        g_print ("couldn't load %s\n", filename);
        return -1;
    }
    if (g_markup_parse_context_parse (context, text, length, NULL) == FALSE) {
        g_print ("Parse failed\n");
        return -1;
    }
    g_free (text);
    g_markup_parse_context_free (context);

    return 0;
}
