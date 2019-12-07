cmake_minimum_required(VERSION 3.10)
project(1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O2")


add_executable(1 main.cpp Stream.cpp EncDec.cpp HuffmanEncoder.cpp)
