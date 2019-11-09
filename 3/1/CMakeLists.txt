cmake_minimum_required(VERSION 3.10)
project(3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O2")
set(CMAKE_CXX_STANDARD 14)

add_executable(test test.cpp)