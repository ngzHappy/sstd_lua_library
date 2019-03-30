#pragma once

#include "../sstd_lua_library_global.hpp"

#include <list>
#include <unordered_map>

class LuaRegisterTable;

class LuaRegisterTable {
public:

    using FunctionMap = std::unordered_map<
        std::string_view,
        lua_CFunction,
        std::hash<std::string_view>,
        std::equal_to<>,
        sstd::allocator<std::pair<const std::string_view, lua_CFunction>>>;

    typedef void(*PushValueFunction)(lua_State *);
    using KeyValueArray = std::list< std::pair< PushValueFunction, PushValueFunction> >;


private:
    lua_State * thisL;
    std::string_view thisTableName;
    FunctionMap * thisFunctionMap;
    KeyValueArray * thisKeyValueArray;
public:
    inline operator lua_State *() const;
    inline operator FunctionMap *() const;
public:
    LuaRegisterTable(lua_State *L,
        std::string_view argTableName/*used for debug*/,
        FunctionMap *,
        KeyValueArray * /**/=nullptr);
public:
    void createTable();
private:
    friend void _createTable(LuaRegisterTable * arg);
private:
    sstd_class(LuaRegisterTable);
};

inline LuaRegisterTable::operator lua_State *() const {
    return thisL;
}

inline LuaRegisterTable::operator FunctionMap *() const {
    return thisFunctionMap;
}



