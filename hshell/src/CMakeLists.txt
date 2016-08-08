INCLUDE_DIRECTORIES(${DEPS_INCLUDE_DIRS})
LINK_DIRECTORIES(${DEPS_LIBRARY_DIRS})

SET (hshell_SOURCES 
    hs-app.c 
    hs-app-chooser.c 
    hs-dash.c 
    hs-date.c
    hs-desktop-proc.c 
    hs-ext.c 
    hs-ext-manager.c 
    hs-filter.c
    hs-launcher-button.c 
    hs-main.c
    hs-marshal.c 
    hs-message.c
    hs-misc.c 
    hs-overview.c 
    hs-overview-window.c 
    hs-panel.c 
    hs-power.c
    hs-run.c 
    hs-user-button.c
    hs-x.c
)

SET (hshell_HEADERS
    hs-app.h 
    hs-app-chooser.h 
    hs-dash.h 
    hs-date.h
    hs-desktop-proc.h 
    hs-ext.h 
    hs-ext-manager.h 
    hs-filter.h
    hs-launcher-button.h 
    hs-marshal.h 
    hs-message.h
    hs-misc.h 
    hs-overview.h 
    hs-overview-window.h 
    hs-panel.h 
    hs-power.h
    hs-run.h 
    hs-user-button.h
    hs-x.h
)

SET(CMAKE_C_FLAGS "-std=c99 -Wall")
IF(_DEBUG)
   SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -DDEBUG")
ENDIF(_DEBUG)

ADD_EXECUTABLE (hshell ${hshell_SOURCES} ${hshell_HEADERS})
INSTALL(TARGETS hshell RUNTIME DESTINATION bin)
TARGET_LINK_LIBRARIES (hshell ${DEPS_LIBRARIES})
