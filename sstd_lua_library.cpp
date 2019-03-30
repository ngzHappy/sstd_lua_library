#include "sstd_lua_library_global.hpp"
#include "sstd_lua_filesystem/sstd_lua_filesystem.hpp"
#include "sstd_lua_register_table/sstd_lua_register_table.hpp"

inline static LuaRegisterTable::FunctionMap * getTable() {
    static LuaRegisterTable::FunctionMap varAns = []() {
        LuaRegisterTable::FunctionMap varAns;

        varAns["testHellowWorld"sv] = [](lua_State *L) ->int{
            luaL_dostring(L,u8R"(print("Hellow World!"))");
            return 0;
        };

        return std::move(varAns);
    }();
    return &varAns;
}

extern "C" {

    SSTD_SYMBOL_EXPORT int luaopen_sstd_lua_library(lua_State * L) {
        LuaRegisterTable varRegister{L,"sstd"sv,getTable() };
        varRegister.createTable();
        return 1;
    }

}/*extern "C"*/



















