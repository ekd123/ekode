cmake_minimum_required(VERSION 2.8)
PROJECT(hshell C)
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

# Configure
CONFIGURE_FILE(config.h.in config.h)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

# Uninstall
ADD_CUSTOM_TARGET(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# Options
OPTION(ENABLE_DEBUG "Enable Debug" OFF)
IF(ENABLE_DEBUG)
    SET(_DEBUG)
ENDIF(ENABLE_DEBUG)

# Find modules
PKG_CHECK_MODULES(GTK3 REQUIRED "gtk+-3.0 >= 3.4.0")
PKG_CHECK_MODULES(LUA51 REQUIRED "lua-5.1 >= 5.1.0")
PKG_CHECK_MODULES(TRAY REQUIRED "eggtraymanager")

SET(DEPS_INCLUDE_DIRS ${GTK3_INCLUDE_DIRS} ${LUA51_INCLUDE_DIRS} ${TRAY_INCLUDE_DIRS})
SET(DEPS_LIBRARY_DIRS ${GTK3_LIBRARY_DIRS} ${LUA51_LIBRARY_DIRS} ${TRAY_LIBRARY_DIRS})
SET(DEPS_LIBRARIES ${GTK3_LIBRARIES} ${LUA51_LIBRARIES} ${TRAY_LIBRARIES})

# Sub directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (share)
ADD_SUBDIRECTORY (po)
