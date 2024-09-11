#ifdef __cplusplus
  #include "lua.hpp"
#else
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
#endif
#include <math.h>

// https://chsasank.com/lua-c-wrapping.html
// http://milkpot.sakura.ne.jp/lua/lua51_manual_ja.html
// http://milkpot.sakura.ne.jp/lua/lua52_manual_ja.html
// http://milkpot.sakura.ne.jp/lua/lua53_manual_ja.html

//so that name mangling doesn't mess up function names
#ifdef __cplusplus
extern "C"{
#endif

#include "swap.h"
#include "json.h"

static int s_swap(lua_State *L) {
    //check and fetch the arguments
    int arg1 = luaL_checkinteger (L, 1);
    int arg2 = luaL_checkinteger (L, 2);

    swap( &arg1, &arg2 );

    //push the results
    lua_pushnumber(L, arg1);
    lua_pushnumber(L, arg2);

    //return number of results
    return 2;
}

static int s_json(lua_State *L) {
    size_t l = 0;
    int from = 0;
    int to =0;

    const char* target = luaL_checklstring( L, 1, &l);
    char ret[l + JSON_MAX_DEPTH_6];
    ret[0] = '\0';
    bool suc = false;
    suc = parse_json( target, l, ret, &from, &to );

    //pushed string is copied and used on lua side
    lua_pushstring(L, ret);
    lua_pushboolean(L, suc);
    lua_pushnumber(L, from);
    lua_pushnumber(L, to);

    return 4;
}

static int s_yaml(lua_State *L) {
    double arg = luaL_checknumber (L, 1);
    lua_pushnumber(L, sin(arg));
    return 1;
}

//library to be registered
static const struct luaL_Reg mylib [] = {
      {"swap", s_swap},
      {"json", s_json},
      {"yaml", s_yaml},
      {NULL, NULL}  /* sentinel */
    };

//name of this function is not flexible
int luaopen_salvager(lua_State *L){
    luaL_register(L, "salvager", mylib);
    return 1;
}

#ifdef __cplusplus
}
#endif
