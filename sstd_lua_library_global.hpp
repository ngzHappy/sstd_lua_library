#pragma once

#include "sstd_library.hpp"

#if defined(_DEBUG)
#define luaopen_sstd_lua_library luaopen_sstd_lua_library_debug
#else
#define luaopen_sstd_lua_library luaopen_sstd_lua_library
#endif

extern "C" {
    SSTD_SYMBOL_EXPORT int luaopen_sstd_lua_library(lua_State *);
}









