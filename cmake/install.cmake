if(UNIX AND NOT APPLE)
    include(install_unix)
elseif(WIN32)
    include(install_windows)
endif()
