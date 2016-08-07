#include <stdio.h>
#include "core.h"
#include "builtins.h"

/**
   'a
*/
void test_atom ()
{
    printf ("============= TEST ATOM =============\n");
    struct WenObject *atom = wen_create_symbol ("a");
    printf ("is it an atom? %s\n", (wen_buildtin_atomp(atom))?"yes":"no");
    printf ("symbol name: %s\n", atom->symbol_name);
    wen_unref_object (atom);
}

/**
   (a . (b . nil)) or (a b)
*/
void test_list ()
{
    printf ("============= TEST LIST =============\n");
    struct WenObject *list = wen_create_list (wen_create_symbol ("a"), wen_create_list (wen_create_symbol ("b"), NULL));
    printf ("is it a list? %s\n", (wen_buildtin_listp(list))?"yes":"no");
    wen_dump_list (list);
    puts ("");
    wen_unref_object (list);
}

/**
   "hi world"
*/
void test_string ()
{
    printf ("============= TEST STRING =============\n");
    struct WenObject *str = wen_create_string ("hi world");
    printf ("is it an atom? %s\n", (wen_buildtin_atomp(str))?"yes":"no");
    printf ("is it a string? %s\n", (wen_buildtin_stringp(str))?"yes":"no");
    printf ("contents: %s\n", (char*)str->attached_data);
    wen_unref_object (str);
}

void run_tests ()
{
    test_atom ();
    test_list ();
    test_string ();
}

int main (int argc, char *argv[])
{
    run_tests ();
    return 0;
}
