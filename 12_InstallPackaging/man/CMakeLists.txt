# Генерация man-страницы из шаблона
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/mastermind.1.in
    ${CMAKE_BINARY_DIR}/man/mastermind.1
    @ONLY
)

# Установка man-страницы
install(
    FILES ${CMAKE_BINARY_DIR}/man/mastermind.1
    DESTINATION share/man/man1
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)

# Добавление в список целей
add_custom_target(manpage ALL DEPENDS ${CMAKE_BINARY_DIR}/man/mastermind.1)