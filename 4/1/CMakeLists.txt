cmake_minimum_required(VERSION 3.15)
project(1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -fno-stack-limit")

add_executable(1 main.cpp)