cmake_minimum_required(VERSION 3.10)
project(aho)

link_directories("/opt/lib/lib")
include_directories("/opt/lib/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)

add_executable(aho main.cpp)
add_executable(ok ok.cpp)