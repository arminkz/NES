cmake_minimum_required(VERSION 3.10)

project(NES)

find_package(fmt CONFIG REQUIRED)

add_executable(NES src/main.cpp)

target_link_libraries(NES PRIVATE fmt::fmt)

message(STATUS "I hate myself so I use cmake!")