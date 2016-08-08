SET(defconf_FILES
    main.lua
    metadata.ini
)

INSTALL(FILES ${defconf_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/hshell/extensions/main/)

