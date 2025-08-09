cmake_minimum_required(VERSION 4.0)
project(TrackingSandbox)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(TrackingSandbox src/main.cpp
        include/tracking_sandbox/cue_list_interface.h
        src/channel.h
        src/cue.h
        src/cue_list.h
        src/channel.cpp
        src/cue.cpp
        src/cue_list.cpp
        src/move_instruction.cpp
        src/move_instruction.h
        src/console_interface.h
        src/console_interface.cpp)
