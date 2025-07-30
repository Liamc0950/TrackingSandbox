cmake_minimum_required(VERSION 3.10.0)
project(TrackingSandbox VERSION 0.1.0 LANGUAGES C CXX)

add_library(TrackingSandbox TrackingSandbox.cpp)

include(CTest)
enable_testing()

