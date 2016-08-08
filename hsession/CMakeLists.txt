cmake_minimum_required(VERSION 2.8)
PROJECT(hsession C)
SET (CMAKE_C_COMPILER clang)

# Pkgconfig
FIND_PACKAGE(PkgConfig)

# Internationalization
FIND_PACKAGE(Gettext REQUIRED)
FIND_PROGRAM(INTLTOOL_EXTRACT intltool-extract)
FIND_PROGRAM(INTLTOOL_UPDATE intltool-update)
FIND_PROGRAM(INTLTOOL_MERGE intltool-merge)
set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")

# Uninstall
ADD_CUSTOM_TARGET(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Find modules
PKG_CHECK_MODULES(GIO REQUIRED "gio-unix-2.0 >= 2.32.0")

SET(DEPS_INCLUDE_DIRS ${GIO_INCLUDE_DIRS})
SET(DEPS_LIBRARY_DIRS ${GIO_LIBRARY_DIRS})
SET(DEPS_LIBRARIES ${GIO_LIBRARIES})

# Sub directories
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (po)

