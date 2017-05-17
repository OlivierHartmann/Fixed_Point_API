# CMake entry point
cmake_minimum_required (VERSION 3.0.2)

project (Fixed_Point_API)

set (EXECUTABLE_OUTPUT_PATH bin/)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

# Generate the source files list
file (GLOB_RECURSE source_files src/*)

# Binary
add_executable (Fixed_Point_API ${source_files})