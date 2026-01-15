cmake_minimum_required(VERSION 3.10)
project(buf VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)

add_compile_options(-Wall -Wextra)

if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

add_library(buf STATIC lib.c)
target_include_directories(buf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


if(BUILD_TESTS)
    enable_testing()
    
    add_executable(test_basic test_basic.c)
    target_link_libraries(test_basic buf)
    add_test(NAME basic_tests COMMAND test_basic)
    
    add_executable(test_push_pop test_push_pop.c)
    target_link_libraries(test_push_pop buf)
    add_test(NAME push_pop_tests COMMAND test_push_pop)
    
    add_executable(test_grow_trunc test_grow_trunc.c)
    target_link_libraries(test_grow_trunc buf)
    add_test(NAME grow_trunc_tests COMMAND test_grow_trunc)

    add_custom_target(check ALL DEPENDS test_basic test_push_pop test_grow_trunc)

    if(ENABLE_COVERAGE)
        find_program(GCOVR gcovr)
        if(GCOVR)
            add_custom_target(coverage
                COMMAND ${GCOVR} 
                    --exclude ".*test.*.c"
                    --html-details ${CMAKE_BINARY_DIR}/coverage.html
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating coverage report"
            )
            add_dependencies(coverage check)
        endif()
        
        add_custom_target(coverage-simple
            COMMAND ${GCOVR} --exclude ".*test.*.c"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()
