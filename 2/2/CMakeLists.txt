cmake_minimum_required(VERSION 3.14)
project(2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak  -fsanitize=undefined")
set(CMAKE_CXX_STANDARD 14)

add_executable(2 main.cpp part.h)
add_executable(test test.cpp part.h)