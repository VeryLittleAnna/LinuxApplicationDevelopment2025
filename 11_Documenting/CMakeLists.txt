cmake_minimum_required(VERSION 3.10)
project(binsearch VERSION 1.0 LANGUAGES C)

find_package(Gettext REQUIRED)
find_package(Intl REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LOCALEDIR "${CMAKE_BINARY_DIR}/locale")

add_definitions(-DLOCALEDIR="${CMAKE_BINARY_DIR}/locale")

add_subdirectory(src)
add_subdirectory(po)

if(EXISTS ${CMAKE_SOURCE_DIR}/docs)
    add_subdirectory(docs)
endif()