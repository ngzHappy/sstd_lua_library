﻿
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

    inline static int setCurrentPath(lp L);

    class Path {
    public:
        sstd::filesystem::path thisData;
    public:

        inline static int createPath(lp L) {

            std::size_t varLength{ 1 };
            const char * varAns =
                ::lua_tolstring(L, -1, &varLength);

            if (varAns == nullptr) {
                pushString(L, "arg-1 is not string"sv);
                ::lua_error(L);
            }

            LuaRegisterTable varRegister{ L,
                 sstd_new<Path>(fromUtf8({varAns,varLength})),
                  [](void * arg) { delete reinterpret_cast<Path *>(arg); },
                   "path"sv,
                   getTable(),
                   getArray()
            };
            varRegister.createTable();

            return 1;

        }

        inline static int toString(lp L);

        inline static LuaRegisterTable::FunctionMap * getTable();
        inline static LuaRegisterTable::KeyValueArray  * getArray();

    private:
        sstd_class(Path);
    };

    inline static int setCurrentPath(lp L) {
        sstd_try{
            if (lua_istable(L,-1)) {
                Path * varPath =
                    sstd::luaCheckTableUserData<Path>(L,-1,"path"sv);
                sstd::filesystem::current_path(varPath->thisData);
            } else {
                std::size_t varLength{ 1 };
                auto varAns = ::lua_tolstring(L, -1, &varLength);
                if (!varAns) {
                    pushString(L, "arg-1 is not string or path"sv);
                    ::lua_error(L);
                }
                sstd::filesystem::current_path(fromUtf8({ varAns,varLength }));
            }
        } sstd_catch(std::exception & e) {
            pushString(L, e.what());
            ::lua_error(L);
        }
        return 0;
    }


    inline int Path::toString(lp L) {

        if (false == lua_istable(L, -1)) {
            pushString(L, "arg-1 is not table"sv);
            ::lua_error(L);
        }

        auto varPath =
            reinterpret_cast<Path*>(::lua_gettable_userdata(L, -1));

        assert(varPath);

        auto varString = toUtf8(varPath->thisData);
        pushString(L, varString);
        return 1;

    }

    inline LuaRegisterTable::FunctionMap * Path::getTable() {
        static LuaRegisterTable::FunctionMap varAns = []() {
            auto varAns = LuaRegisterTable::globalFunctionMap();

            using namespace _theSSTDLuaFilesystemFile;

            varAns["toString"sv] = &toString;

            return std::move(varAns);
        }();
        return &varAns;
    }

    inline LuaRegisterTable::KeyValueArray * Path::getArray() {
        return nullptr;
    }

}/**/

inline static LuaRegisterTable::FunctionMap * getTable() {

    static LuaRegisterTable::FunctionMap varAns = []() {
        auto varAns = LuaRegisterTable::globalFunctionMap();

        using namespace _theSSTDLuaFilesystemFile;

        varAns["getCurrentPath"sv] = &getCurrentPath;
        varAns["setCurrentPath"sv] = &setCurrentPath;
        varAns["createPath"sv] = &Path::createPath;

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



