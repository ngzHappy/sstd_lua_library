﻿#pragma once

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
    const FunctionMap * thisFunctionMap;
    const KeyValueArray * thisKeyValueArray;
    void * thisUserData;
    void(*thisUserDataFunction)(void *);
public:
    inline operator lua_State *() const;
    inline operator const FunctionMap *() const;
public:
    static const char * thisIndex();
public:
    LuaRegisterTable(lua_State *L,
        void *,
        void(*)(void*),
        std::string_view argTableName/*used for debug*/,
        const FunctionMap *,
        const KeyValueArray * /**/ = nullptr);
    inline LuaRegisterTable(lua_State *L,
        std::string_view argTableName/*used for debug*/,
        const FunctionMap * argMap,
        const KeyValueArray * argArray = nullptr) :LuaRegisterTable(L,
            nullptr,
            nullptr,
            argTableName,
            argMap,
            argArray){}
public:
    static FunctionMap globalFunctionMap();
public:
    void createTable();
private:
    friend void _createTable(LuaRegisterTable * arg);
    sstd_delete_copy_create(LuaRegisterTable);
private:
    sstd_class(LuaRegisterTable);
};

inline LuaRegisterTable::operator lua_State *() const {
    return thisL;
}

inline LuaRegisterTable::operator const FunctionMap *() const {
    return thisFunctionMap;
}



