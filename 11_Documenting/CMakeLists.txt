cmake_minimum_required(VERSION 3.10)
project(binsearch VERSION 1.0 LANGUAGES C)

find_package(Gettext REQUIRED)
find_package(Intl REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LOCALEDIR "${CMAKE_BINARY_DIR}/locale")

add_definitions(-DLOCALEDIR="${CMAKE_BINARY_DIR}/locale")

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs")
    
    configure_file(
        ${CMAKE_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating Doxygen documentation"
    )

    add_custom_target(view-docs
        COMMAND ${CMAKE_COMMAND} -E echo "Documentation generated at: ${DOXYGEN_OUTPUT_DIR}/html/index.html"
        DEPENDS docs
    )
else()
    message(WARNING "Doxygen not found! Documentation will not be generated.")
endif()

add_subdirectory(src)
add_subdirectory(po)

if(EXISTS ${CMAKE_SOURCE_DIR}/docs)
    add_subdirectory(docs)
endif()