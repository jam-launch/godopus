target_sources( ${PROJECT_NAME}
    PRIVATE
        GodOpusEncoder.cpp
        GodOpusDecoder.cpp
        RegisterExtension.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE
        "src"
)

target_compile_definitions( ${PROJECT_NAME}
    PUBLIC
)