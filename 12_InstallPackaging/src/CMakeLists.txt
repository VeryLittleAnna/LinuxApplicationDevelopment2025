if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(LOCALEDIR "${CMAKE_BINARY_DIR}/locale")
else()
    set(LOCALEDIR "${CMAKE_INSTALL_PREFIX}/share/locale")
endif()

add_definitions(-DLOCALEDIR="${LOCALEDIR}")

add_subdirectory(lib)

add_executable(mastermind main.c)

target_link_libraries(mastermind mastermind_lib)

target_include_directories(mastermind PRIVATE lib)

if(WITH_GETTEXT)
    target_link_libraries(mastermind ${Intl_LIBRARIES})
    target_include_directories(mastermind PRIVATE ${Intl_INCLUDE_DIRS})
    add_dependencies(mastermind locale)
endif()