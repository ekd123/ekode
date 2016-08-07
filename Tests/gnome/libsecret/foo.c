#include <libsecret/secret.h>
#include "foo.h"

const SecretSchema*
foo_get_schema(void)
{
	static const SecretSchema schema = {
		"prj.NekoGroup.Database.Password", SECRET_SCHEMA_NONE,
		{
			{ "NULL", 0 }
		}
	};
	return &schema;
}

int main ()
{
	GError *error = NULL;
	gchar *password = secret_password_lookup_sync (FOO_SCHEMA,
		NULL, &error, NULL);
	if (error != NULL) {
		g_critical ("%s\n", error->message);
		g_error_free (error);
		return 1;
	}
	g_message ("Password %s\n", password);
	secret_password_free (password);
	return 0;
}


