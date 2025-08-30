cmake_minimum_required(VERSION 3.20)
project(cue_list_wasm)

set(CMAKE_CXX_STANDARD 20)

# Add all your source files
add_executable(cue_list_wasm
        src/cue_list.cpp
        src/cue.cpp
        src/channel.cpp
        src/move_instruction.cpp
        src/cue_list_wrapper.cpp
)

# Emscripten link flags
target_link_options(cue_list_wasm PRIVATE
        "SHELL:-s WASM=1"
        "SHELL:-s MODULARIZE=1"
        "SHELL:-s EXPORT_NAME='CueListModule'"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:--bind"
)

# Output .js file instead of default
set_target_properties(cue_list_wasm PROPERTIES SUFFIX ".js")