enable_testing()

add_executable(test_basic test_basic.c)
target_link_libraries(test_basic mastermind_lib)
target_include_directories(test_basic PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
add_test(NAME basic_tests COMMAND test_basic)

add_executable(test_in_output test_in_output.c)
target_link_libraries(test_in_output mastermind_lib)
target_include_directories(test_in_output PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
add_test(NAME io_tests COMMAND test_in_output)

add_custom_target(test
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_basic test_in_output
    COMMENT "Running all tests..."
    VERBATIM
)
