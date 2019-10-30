cmake_minimum_required(VERSION 3.10)
project(1)

set(CMAKE_CXX_FLAGS "-O0 -g -fsanitize=address")
set(CMAKE_CXX_STANDARD 14)

add_executable(1 main.cpp)
add_executable(1_stl main_stl.cpp)