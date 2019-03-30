
#include "sstd_lua_filesystem.hpp"
#include <unordered_map>

class FunctionItem {
public:
    lua_CFunction function;
};

inline static lua_CFunction getFunction(std::string_view key) {

    using Map = std::unordered_map< std::string_view,
        FunctionItem,
        std::hash<std::string_view>,
        std::equal_to<>,
        sstd::allocator< std::pair<const std::string_view, FunctionItem>>>;

    const static Map varAns = []()-> Map {
        Map varAns;

        varAns.emplace("testHellowWorld"sv,
            FunctionItem{ [](lua_State * L)->int {
         ::luaL_loadstring(L,u8R"( print( "Hellow World!" ) )");
         ::lua_pcall(L,0,0,0);
        return 0; } });

        return std::move(varAns); }();

        auto varPos = varAns.find(key);
        if (varPos != varAns.end()) {
            return varPos->second.function;
        }
        return nullptr;
}

inline static int __index(lua_State * L){
    std::size_t varLength{ 1 };
    const char * varAns = ::lua_tolstring(L, 2, &varLength);
    auto varFunction = getFunction({ varAns,varLength });
    if (varFunction) {
        ::lua_pushcclosure(L, varFunction, 0);
    } else {
        ::lua_pushnil(L);
    }
    return 1;
}

extern void pushFilesystemTable(lua_State * L){

    ::lua_createtable(L, 0, 8);
    auto varTableIndex = ::lua_gettop(L);

    ::lua_pushvalue(L, varTableIndex);
    ::lua_setmetatable(L, varTableIndex);

    ::lua_pushlstring(L, "__index", 7);
    ::lua_pushcclosure(L, &__index, 0);
    ::lua_settable(L, varTableIndex);

    ::lua_pushlstring(L, "__name", 6);
    ::lua_pushlstring(L, "filesystem", 17);
    ::lua_settable(L, varTableIndex);

    return;

}

