#include <lualib.h>
#include <lauxlib.h>

int main (void)
{
   lua_State *vm;
   const char *var;
   
   vm = lua_open ();
   luaL_openlibs (vm);
   (void)luaL_dofile (vm, "./a.lua");
   (void)luaL_dofile (vm, "./b.lua");
   lua_getglobal (vm, "value");
   var = lua_tostring (vm, -1);
   printf ("%s\n", var);
   lua_pop (vm, 1);
   return 0;
}

