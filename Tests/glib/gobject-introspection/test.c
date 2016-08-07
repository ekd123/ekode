#include <glib.h>
#include <glib-object.h>
#include <girepository.h>
#include <lualib.h>
#include <lauxlib.h>

int main (int argc, char *argv[])
{
    gchar *gir_path  = "./";
    lua_State *vm;

    g_type_init ();

    GOptionContext *ctx = g_option_context_new (NULL);
    g_option_context_add_group (ctx, g_irepository_get_option_group ());
    g_option_context_parse (ctx, &argc, &argv, NULL);
    
    /* 设置 typelib 文件查询路径 */
    g_irepository_prepend_search_path (gir_path);

    vm = lua_open ();
    luaL_openlibs (vm);
    luaL_dofile (vm, "./a.lua");
    lua_close (vm);
    return 0;
}
