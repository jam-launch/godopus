# SPDX-License-Identifier: Unlicense

add_custom_target( templates
    SOURCES
        template.gdextension.in
)

add_dependencies( ${PROJECT_NAME} templates )

set( GD_EXTENSION_FILE_INPUT template.gdextension.in )

if ( MINGW )
    set( LIB_PREFIX "lib")
endif()

set( GD_EXTENSION_FILE ${PROJECT_NAME}.gdextension )
configure_file( ${GD_EXTENSION_FILE_INPUT} ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${GD_EXTENSION_FILE} )

install(
    FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${GD_EXTENSION_FILE}
    DESTINATION ${INSTALL_DIR}
)

unset( GD_EXTENSION_FILE )
unset( GD_EXTENSION_FILE_INPUT )
unset( LIB_PREFIX )
