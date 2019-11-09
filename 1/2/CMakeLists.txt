cmake_minimum_required(VERSION 3.10)
project(2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_CXX_STANDARD 14)

add_executable(2 main.cpp)
add_executable(2_stl main_stl.cpp)