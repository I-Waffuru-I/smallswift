cmake_minimum_required(VERSION 3.16)
project(comp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(libs/libcomp)
add_subdirectory(libs/liblexer)
add_subdirectory(libs/libparser)
add_subdirectory(src)
add_subdirectory(tools)
