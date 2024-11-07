cmake_minimum_required( VERSION 3.24 )

message( STATUS "Using CMake ${CMAKE_VERSION}" )

set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )

if ( APPLE )
    set( CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" )
endif()

# Define project
project( godopus
    LANGUAGES
        CXX
    VERSION
        0.1.0
)
set( LIB_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
if ( APPLE )
    set( LIB_ARCH "universal" )
endif()

set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/dist" CACHE PATH "default install path" FORCE)

set( LIB_DIR "lib/${CMAKE_SYSTEM_NAME}-${LIB_ARCH}" )
set( INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/" )

# Dependencies
add_subdirectory(opus)
add_subdirectory(godot-cpp)

# Create library
add_library( ${PROJECT_NAME} SHARED )

target_compile_features( ${PROJECT_NAME}
    PRIVATE
        cxx_std_17
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        godot-cpp
        opus
)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true
)
if ( NOT MSVC )
    if(APPLE)
        set(MACOSX_RPATH TRUE)
        set_target_properties( ${PROJECT_NAME}
            PROPERTIES
                INSTALL_RPATH "@executable_path"
        )
        target_link_options( ${PROJECT_NAME} PRIVATE
            "-Wl,-undefined,error"
            "-Wl,-dead_strip"
        )
    else()
        set_target_properties( ${PROJECT_NAME}
            PROPERTIES
                INSTALL_RPATH "$ORIGIN"
        )
        target_link_options( ${PROJECT_NAME} PRIVATE
            "-Wl,--no-undefined"
            "-Wl,--gc-sections"
            "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/version.script"
            -static-libgcc
            -static-libstdc++
        )
    endif()
    target_compile_options( ${PROJECT_NAME} PRIVATE
        -ffunction-sections
        -fdata-sections
    )
endif()

add_subdirectory( src )

message( STATUS "Install directory: ${INSTALL_DIR}")
install( TARGETS ${PROJECT_NAME}
    LIBRARY
        DESTINATION ${INSTALL_DIR}/${LIB_DIR}
    RUNTIME
        DESTINATION ${INSTALL_DIR}/${LIB_DIR}
)

add_subdirectory( templates )
