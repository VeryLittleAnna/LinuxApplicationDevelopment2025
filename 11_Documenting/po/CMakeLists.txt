set(PO_DIR "${CMAKE_SOURCE_DIR}/po")
set(MO_DIR "${CMAKE_BINARY_DIR}/locale")

file(MAKE_DIRECTORY ${MO_DIR}/ru/LC_MESSAGES)

add_custom_command(
    OUTPUT ${MO_DIR}/ru/LC_MESSAGES/binsearch.mo
    COMMAND msgfmt --output-file=${MO_DIR}/ru/LC_MESSAGES/binsearch.mo ${PO_DIR}/ru.po
    DEPENDS ${PO_DIR}/ru.po
    COMMENT "Building Russian translation file"
)

add_custom_target(translations ALL
    DEPENDS ${MO_DIR}/ru/LC_MESSAGES/binsearch.mo
)

add_custom_target(pot
    COMMAND xgettext --keyword=_ --keyword=N_ --language=C -o ${PO_DIR}/binsearch.pot ${CMAKE_SOURCE_DIR}/src/binsearch.c
    COMMENT "Extracting messages to pot file"
)

add_custom_target(update_po
    COMMAND msgmerge --update ${PO_DIR}/ru.po ${PO_DIR}/binsearch.pot
    DEPENDS pot
    COMMENT "Updating Russian translations"
)

install(
    DIRECTORY ${MO_DIR}/
    DESTINATION share/locale
    FILES_MATCHING PATTERN "*.mo"
)