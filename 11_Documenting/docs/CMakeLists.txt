configure_file(
    ${CMAKE_SOURCE_DIR}/docs/binsearch.1.in
    ${CMAKE_BINARY_DIR}/docs/binsearch.1
    @ONLY
)

install(
    FILES ${CMAKE_BINARY_DIR}/docs/binsearch.1
    DESTINATION share/man/man1
)