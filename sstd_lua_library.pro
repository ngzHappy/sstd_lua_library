
TEMPLATE = lib

QT -= core
QT -= gui

include($$PWD/../sstd_library/sstd_library.pri)
DESTDIR = $$SSTD_LIBRARY_OUTPUT_PATH

CONFIG(debug,debug|release){
    TARGET = sstd_lua_library_debug
} else{
    TARGET = sstd_lua_library
}

HEADERS += $$PWD/sstd_lua_library_global.hpp
SOURCES += $$PWD/sstd_lua_library.cpp

HEADERS += $$PWD/sstd_lua_filesystem/sstd_lua_filesystem.hpp
SOURCES += $$PWD/sstd_lua_filesystem/sstd_lua_filesystem.cpp

HEADERS += $$PWD/sstd_lua_register_table/sstd_lua_register_table.hpp
SOURCES += $$PWD/sstd_lua_register_table/sstd_lua_register_table.cpp






