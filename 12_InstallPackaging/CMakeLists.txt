cmake_minimum_required(VERSION 3.10)
project(mastermind VERSION 1.0.0 LANGUAGES C)

option(BUILD_SHARED_LIBS "Построить shared библиотеку" ON)
option(BUILD_TESTS "Построить тесты" ON)
option(BUILD_DOCS "Построить документацию" OFF)
option(WITH_GETTEXT "Включить поддержку gettext" ON)

add_compile_options(-Wall -Wextra -Werror)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WITH_GETTEXT)
    find_package(Intl REQUIRED)
    find_package(Gettext REQUIRED)
    find_program(MSGFMT msgfmt)
    add_definitions(-DWITH_GETTEXT)
    add_subdirectory(po)
endif()

add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs")
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_VERSION "${PROJECT_VERSION}")
    set(DOXYGEN_PROJECT_BRIEF "Игра Mastermind (Быки и коровы)")
    
    configure_file(
        ${CMAKE_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Генерация документации Doxygen"
        VERBATIM
    )
    
    if(BUILD_DOCS)
        # Установка будет работать только после make docs
        install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html
            DESTINATION share/doc/mastermind
            OPTIONAL
        )
    endif()
    
else()
    message(WARNING "Doxygen not found!")
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/man)
    add_subdirectory(man)
endif()

install(TARGETS mastermind mastermind_lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY src/lib/ DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/locale/
    DESTINATION share/locale
    USE_SOURCE_PERMISSIONS
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/locale/
    DESTINATION share/locale
    FILES_MATCHING PATTERN "*.mo"
)

if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        @ONLY IMMEDIATE
    )
    
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        COMMENT "Удаление установленных файлов"
    )
endif()