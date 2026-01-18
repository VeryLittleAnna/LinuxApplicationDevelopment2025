set(PO_DIR "${CMAKE_SOURCE_DIR}/po")
set(MO_DIR "${CMAKE_BINARY_DIR}/locale")

file(MAKE_DIRECTORY ${MO_DIR}/ru/LC_MESSAGES)
file(MAKE_DIRECTORY ${MO_DIR}/en/LC_MESSAGES)

add_custom_target(pot
    COMMAND xgettext
        --keyword=_
        --keyword=N_
        --keyword=gettext:1
        --keyword=ngettext:1,2
        --add-comments
        --from-code=UTF-8
        -o ${PO_DIR}/mastermind.pot
        ${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/lib/mastermind.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Extracting messages to pot file"
)

add_custom_target(update_po
    COMMAND msgmerge --update ${PO_DIR}/ru.po ${PO_DIR}/mastermind.pot
    COMMAND msgmerge --update ${PO_DIR}/en.po ${PO_DIR}/mastermind.pot
    DEPENDS pot
    COMMENT "Updating translation files"
)

add_custom_command(
    OUTPUT ${MO_DIR}/ru/LC_MESSAGES/mastermind.mo
    COMMAND msgfmt --output-file=${MO_DIR}/ru/LC_MESSAGES/mastermind.mo ${PO_DIR}/ru.po
    DEPENDS ${PO_DIR}/ru.po
    COMMENT "Building Russian translation file"
)

add_custom_command(
    OUTPUT ${MO_DIR}/en/LC_MESSAGES/mastermind.mo
    COMMAND msgfmt --output-file=${MO_DIR}/en/LC_MESSAGES/mastermind.mo ${PO_DIR}/en.po
    DEPENDS ${PO_DIR}/en.po
    COMMENT "Building English translation file"
)

add_custom_target(locale ALL
    DEPENDS 
        ${MO_DIR}/ru/LC_MESSAGES/mastermind.mo
        ${MO_DIR}/en/LC_MESSAGES/mastermind.mo
    COMMENT "Building all translations"
)

install(
    DIRECTORY ${MO_DIR}/
    DESTINATION share/locale
    FILES_MATCHING PATTERN "*.mo"
)