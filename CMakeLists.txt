cmake_minimum_required(VERSION 3.20)
project(cue_list_wasm)

set(CMAKE_CXX_STANDARD 20)

# Check if we're building with Emscripten
if(EMSCRIPTEN)
    # Emscripten build
    add_executable(cue_list_wasm
            src/cue_list.cpp
            src/cue.cpp
            src/channel.cpp
            src/move_instruction.cpp
            src/cue_list_wrapper.cpp
    )

    target_link_options(cue_list_wasm PRIVATE
            "SHELL:-s WASM=1"
            "SHELL:-s MODULARIZE=1"
            "SHELL:-s EXPORT_NAME='CueListModule'"
            "SHELL:-s ALLOW_MEMORY_GROWTH=1"
            "SHELL:--bind"
    )

    set_target_properties(cue_list_wasm PROPERTIES SUFFIX ".js")
else()
    # Native build for local development
    add_executable(cue_list_native
            src/cue_list.cpp
            src/cue.cpp
            src/channel.cpp
            src/console_interface.cpp
            src/move_instruction.cpp
            src/main.cpp
    )

    # Add any native-specific compiler flags if needed
    target_compile_options(cue_list_native PRIVATE -Wall -Wextra)
endif()

# Google tests - only for native builds
if(NOT EMSCRIPTEN)
    add_subdirectory(Google_tests)
endif()