add_executable(binsearch binsearch.c)

target_link_libraries(binsearch ${Intl_LIBRARIES})
target_include_directories(binsearch PRIVATE ${Intl_INCLUDE_DIRS})

add_dependencies(binsearch translations)

set_target_properties(binsearch PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS binsearch DESTINATION bin)