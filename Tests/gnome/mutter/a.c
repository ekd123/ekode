#include <mutter/meta/main.h>

int main ()
{
   meta_init();
   meta_set_replace_current_wm (1);
   meta_run ();
   meta_quit(1);
   return 0;
}

