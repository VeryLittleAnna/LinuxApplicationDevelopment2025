set(PO_DIR "${CMAKE_SOURCE_DIR}/po")
set(MO_DIR "${CMAKE_BINARY_DIR}/locale")

add_custom_command(
    OUTPUT ${MO_DIR}/ru/LC_MESSAGES/binsearch.mo
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MO_DIR}/ru/LC_MESSAGES
    COMMAND msgfmt -o ${MO_DIR}/ru/LC_MESSAGES/binsearch.mo ${PO_DIR}/ru.po
    DEPENDS ${PO_DIR}/ru.po
    COMMENT "Building Russian translation file")

add_custom_target(translations ALL
    DEPENDS ${MO_DIR}/ru/LC_MESSAGES/binsearch.mo)

add_custom_target(pot
    COMMAND cd ${CMAKE_SOURCE_DIR} && xgettext --keyword=_ --language=C -o po/binsearch.pot -s src/binsearch.c
    COMMENT "Extracting messages to pot file")

add_custom_target(update_po
    COMMAND msgmerge --update ${PO_DIR}/ru.po ${PO_DIR}/binsearch.pot
    DEPENDS pot
    COMMENT "Updating Russian translations")