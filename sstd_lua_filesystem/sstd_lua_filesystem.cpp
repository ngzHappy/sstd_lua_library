
#include "sstd_lua_filesystem.hpp"
#include "sstd_lua_register_table/sstd_lua_register_table.hpp"

#include <unordered_map>

#if defined(_WIN32)
#define WINDOWS_LICK_
#else
#endif

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

    inline static std::string toUtf8(const sstd::filesystem::path & arg) {
#if defined( WINDOWS_LICK_ )
        auto varAns = arg.u8string();
        replace(varAns);
        return std::move(varAns);
#else
        return arg.string();
#endif
    }

    inline static sstd::filesystem::path fromUtf8(std::string_view arg) {
#if defined( WINDOWS_LICK_ )
#if __has_include(<filesystem>)
        return sstd::filesystem::u8path(arg);
#else
        return sstd::filesystem::path(arg)/*TODO:this is logical error!!!*/;
#endif
#else
        return sstd::filesystem::path(arg);
#endif
    }

    inline static int getCurrentPath(lp L) {
        auto varPath = toUtf8(sstd::filesystem::current_path());
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
            sstd::filesystem::current_path(fromUtf8({ varAns,varLength }));
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



