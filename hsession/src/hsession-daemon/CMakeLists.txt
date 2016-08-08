INCLUDE_DIRECTORIES(${DEPS_INCLUDE_DIRS})
LINK_DIRECTORIES(${DEPS_LIBRARY_DIRS})

SET (hsession_daemon_SOURCES
    ../dbus/hs-dbus-manager.c
    hs-autostart.c
    hs-dbus-handler.c
    hs-dbus-proxy.c
    hs-home.c
    hs-main.c
)

SET (hsession_daemon_HEADERS
    ../dbus/hs-dbus-manager.h
    hs-autostart.h
    hs-dbus-handler.h
    hs-dbus-proxy.h
    hs-home.h
)

SET(CMAKE_C_FLAGS "-Wall")

ADD_EXECUTABLE (hsession-daemon ${hsession_daemon_SOURCES} ${hsession_daemon_HEADERS})
INSTALL(TARGETS hsession-daemon RUNTIME DESTINATION bin)
TARGET_LINK_LIBRARIES (hsession-daemon ${DEPS_LIBRARIES})

