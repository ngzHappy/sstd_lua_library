
#include "sstd_lua_filesystem.hpp"
#include "sstd_lua_register_table/sstd_lua_register_table.hpp"

#include <unordered_map>


namespace _theSSTDLuaFilesystemFile {

    using lp = lua_State * ;

    inline static void replace(std::string & arg) {
        for (auto & varI : arg) {
            if (varI == '\\') {
                varI = '/';
            }
        }
    }

    inline static void pushString(lp L, std::string_view arg) {
        ::lua_pushlstring(L, arg.data(), arg.size());
    }

    inline static int getCurrentPath(lp L) {
        auto varPath = sstd::filesystem::current_path().u8string();
#if defined( _WIN32 )
        replace(varPath);
#endif
        ::lua_pushlstring(L, varPath.data(), varPath.size());
        return 1;
    }

    inline static int setCurrentPath(lp L) {
        try {
            constexpr auto varStringIndex = 1;
            std::size_t varLength{ 1 };
            auto varAns = ::lua_tolstring(L,
                varStringIndex, &varLength);
            if (!varAns) {
                pushString(L, "arg_1 is not string"sv);
                ::lua_error(L);
            }
            sstd::filesystem::current_path(sstd::filesystem::u8path(
                varAns, varAns + varLength));
        } catch (std::exception & e) {
            pushString(L, e.what());
            ::lua_error(L);
        }
        return 0;
    }

}/**/


inline static LuaRegisterTable::FunctionMap * getTable() {

    static LuaRegisterTable::FunctionMap varAns = []() {
        LuaRegisterTable::FunctionMap varAns;

        using lp = _theSSTDLuaFilesystemFile::lp;
        using namespace _theSSTDLuaFilesystemFile;

        varAns["testHellowWorld"sv] = [](lua_State *L) ->int {
            luaL_dostring(L, u8R"(print("Hellow World!"))");
            return 0;
        };

        varAns["getCurrentPath"sv] = &getCurrentPath;
        varAns["setCurrentPath"sv] = &setCurrentPath;

        return std::move(varAns);
    }();
    return &varAns;
}

inline static LuaRegisterTable::KeyValueArray  * getArray() {
    return nullptr;
}

extern void sstd::pushFilesystemTable(lua_State * L) {

    LuaRegisterTable varRegister{ L,
           "filesystem"sv,
           getTable(),
           getArray()
    };
    varRegister.createTable();

    return;

}

