#include <libsecret/secret.h>
#include "bar.h"

const SecretSchema*
bar_get_schema(void)
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
	secret_password_store_sync (BAR_SCHEMA, NULL, "Database", 
		"foobarbaz", NULL, &error, NULL);
	if (error != NULL) {
		g_critical ("%s\n", error->message);
		g_error_free (error);
		return 1;
	}
	g_message ("Password added\n");
	return 0;
}


