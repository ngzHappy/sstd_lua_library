#include "sstd_lua_register_table.hpp"

LuaRegisterTable::LuaRegisterTable(lua_State *L,
    void * argUserData,
    void(*argUserFunction)(void*),
    std::string_view argTableName,
    FunctionMap * argMap,
    KeyValueArray * argArray) : thisL(L),
    thisTableName(argTableName),
    thisFunctionMap(argMap),
    thisKeyValueArray(argArray) {
    thisUserData = argUserData;
    thisUserDataFunction = argUserFunction;
}

inline static lua_CFunction _this_get_function(lua_State * L,
    std::string_view key) {

    auto varMap =
        reinterpret_cast<LuaRegisterTable::FunctionMap *>(
            ::lua_touserdata(L, lua_upvalueindex(1)));

    auto varPos = varMap->find(key);
    if (varPos == varMap->end()) {
        return nullptr;
    }

    return varPos->second;

}

inline static int _this_index(lua_State * L) {
    std::size_t varLength{ 1 };
    const char * varAns = ::lua_tolstring(L, 2, &varLength);
    auto varFunction = _this_get_function(L, { varAns,varLength });
    if (varFunction) {
        ::lua_pushcclosure(L, varFunction, 0);
    } else {
        ::lua_pushnil(L);
    }
    return 1;
}

inline void _createTable(LuaRegisterTable * arg) {

    auto & L = arg->thisL;
    auto & thisKeyValueArray = arg->thisKeyValueArray;
    auto & thisFunctionMap = arg->thisFunctionMap;
    auto & thisTableName = arg->thisTableName;

    ::lua_checkstack(L, 64);

    ::lua_createtable(L, 0,
        thisKeyValueArray ?
        static_cast<int>(thisKeyValueArray->size() + 4) : 4);
    auto varTableIndex = ::lua_gettop(L);

    ::lua_settable_userdata(L, varTableIndex,
        arg->thisUserData,
        arg->thisUserDataFunction);

    ::lua_pushvalue(L, varTableIndex);
    ::lua_setmetatable(L, varTableIndex);

    ::lua_pushlstring(L, "__index", 7);
    ::lua_pushlightuserdata(L, thisFunctionMap);
    ::lua_pushcclosure(L, &_this_index, 1);
    ::lua_settable(L, varTableIndex);

    if (thisTableName.empty()) {
        thisTableName = "UnknowType"sv;
    }
    ::lua_pushlstring(L, "__name", 6);
    ::lua_pushlstring(L,
        thisTableName.data(),
        static_cast<int>(thisTableName.size()));
    ::lua_settable(L, varTableIndex);

    if (thisKeyValueArray) {
        for (const auto & varI : *thisKeyValueArray) {
            varI.first(L);
            varI.second(L);
            ::lua_settable(L, varTableIndex);
            ::lua_settop(L, varTableIndex);
        }
    }

    ::lua_settop(L, varTableIndex);
}

inline static int _this_create_table(lua_State * L) {
    _createTable(
        reinterpret_cast<LuaRegisterTable *>(::lua_touserdata(L, 1)));
    return 1;
}

void LuaRegisterTable::createTable() {
    ::lua_pushcclosure(thisL, &_this_create_table, 0);
    ::lua_pushlightuserdata(thisL, this);
    ::lua_pcall(thisL, 1, 1, 0);
    return;
}

const char * LuaRegisterTable::thisIndex() {
    return "thisIndex";
}

LuaRegisterTable::FunctionMap LuaRegisterTable::globalFunctionMap() {
    const static FunctionMap varAns = []() {
        FunctionMap varAns;

        varAns["testHellowWorld"sv] = [](lua_State *L) ->int {
            luaL_dostring(L, u8R"(print("Hellow World!"))");
            return 0;
        };

        return std::move(varAns);
    }();
    return varAns;
}

/*
sstd = require("sstd_lua_library_debug")
sstd.testHellowWorld()
os.exit()
sstd.filesystem.testHellowWorld()
sstd.filesystem.setCurrentPath("C:/")
print( sstd.filesystem.getCurrentPath() )
path = sstd.filesystem.createPath("C:/")
print(path:toString())
*/







