#include <glib.h>

#define RATDB_FIND_REAL_FILENAME(filename)  \
    (g_strndup(filename, g_utf8_strrchr(filename, -1, '.')-filename))

void test(const char *string)
{
	char *result = RATDB_FIND_REAL_FILENAME(string);
	printf ("---\n%s\n", result);
	g_free (result);
}

int main ()
{
	test("HELLO.WORLD.NONEED");	
	return 0;
}
