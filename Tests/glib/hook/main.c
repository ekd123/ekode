#include <glib.h>

GHookList list;

void func1 (gpointer data)
{
    g_print ("Hello world!\n");
}

void destroy1 (gpointer data)
{
    g_print ("hook1 is finalizing\n");
}

void func2 (gpointer data)
{
    g_print ("func2 is invoked, %d\n", GPOINTER_TO_INT(data));
}

void destroy2 (gpointer data)
{
    g_print ("hook2 is finalizing\n");
}

int main (int argc, char *argv [])
{
    g_hook_list_init (&list, sizeof (GHook));
    GHook *hook1 = g_hook_alloc (&list);
    GHook *hook2 = g_hook_alloc (&list);
    hook1->func = func1;
    hook1->destroy = destroy1;
    hook2->func = func2;
    hook2->data = GINT_TO_POINTER(123);
    hook2->destroy = destroy2;
    g_hook_append (&list, hook1);
    g_hook_append (&list, hook2);
    g_hook_list_invoke (&list, FALSE);
    g_hook_list_clear (&list);
    return 0;
}
