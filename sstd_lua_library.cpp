#include "sstd_lua_library_global.hpp"
#include "sstd_lua_filesystem/sstd_lua_filesystem.hpp"
#include "sstd_lua_register_table/sstd_lua_register_table.hpp"

inline static LuaRegisterTable::FunctionMap * getTable() {
    static LuaRegisterTable::FunctionMap varAns = []() {
        auto varAns = LuaRegisterTable::globalFunctionMap();

        return std::move(varAns);
    }();
    return &varAns;
}

inline static LuaRegisterTable::KeyValueArray  * getArray() {
    static LuaRegisterTable::KeyValueArray varAns = []() {
        using p = std::pair< LuaRegisterTable::PushValueFunction,
            LuaRegisterTable::PushValueFunction>;
        using lp = lua_State * ;
        LuaRegisterTable::KeyValueArray varAns{
            p{[](lp L) { ::lua_pushlstring(L,"filesystem",10); }, &sstd::pushFilesystemTable },
        };
        return std::move(varAns);
    }();
    return &varAns;
}

extern "C" {

    SSTD_SYMBOL_EXPORT int luaopen_sstd_lua_library(lua_State * L) {
        LuaRegisterTable varRegister{ L,
            "sstd"sv,
            getTable(),
            getArray()
        };
        varRegister.createTable();
        return 1;
    }

}/*extern "C"*/



















