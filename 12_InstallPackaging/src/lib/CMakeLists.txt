set(MASTERMIND_SOURCES mastermind.c)
set(MASTERMIND_HEADERS mastermind.h)

add_library(mastermind_lib SHARED mastermind.c)

target_include_directories(mastermind_lib PUBLIC .)

if(WITH_GETTEXT AND Intl_FOUND)
    target_link_libraries(mastermind_lib ${Intl_LIBRARIES})
    target_include_directories(mastermind_lib PRIVATE ${Intl_INCLUDE_DIRS})
endif()