cmake_minimum_required(VERSION 3.14)
project(1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

add_executable(1 main.cpp count_inverse.cpp)
add_executable(test_1 test.cpp count_inverse.cpp)