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
#include "multipart.h"

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

static int s_multipart(lua_State *L) {
    size_t l = 0;
    size_t dl = 0;
    int from = 0;
    int to =0;

    const char* target = luaL_checklstring( L, 1, &l);
    const char* delim = luaL_checklstring( L, 2, &dl);

    char key[MULTIPART_KEY_MAX];
    char value[l];
    key[0] = '\0';
    value[0] = '\0';
    bool suc = false;

    suc = parse_multipart( target, delim, l, (char*)key,(char*)value, &from, &to);

    //pushed string is copied and used on lua side
    lua_pushstring(L, key);
    lua_pushstring(L, value);
    lua_pushboolean(L, suc);
    lua_pushnumber(L, from);
    lua_pushnumber(L, to);

    return 5;
}

static int s_multipart_offset(lua_State *L) {
    size_t l = 0;
    size_t dl = 0;
    int from = 0;
    int to =0;

    const char* target = luaL_checklstring( L, 1, &l);
    int offset = luaL_checkinteger (L, 2);
    const char* delim = luaL_checklstring( L, 3, &dl);

    char key[MULTIPART_KEY_MAX];
    char value[l];
    key[0] = '\0';
    value[0] = '\0';
    bool suc = false;

    suc = parse_multipart_offset( target, offset, delim, l, (char*)key,(char*)value, &from, &to);

    //pushed string is copied and used on lua side
    lua_pushstring(L, key);
    lua_pushstring(L, value);
    lua_pushboolean(L, suc);
    lua_pushnumber(L, from);
    lua_pushnumber(L, to);

    return 5;
}

//library to be registered
static const struct luaL_Reg mylib [] = {
      {"swap", s_swap},
      {"json", s_json},
      {"yaml", s_yaml},
      {"multipart", s_multipart},
      {"multipart_offset", s_multipart_offset},
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
