cmake_minimum_required(VERSION 3.10)
project(salesman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native")

add_executable(salesman main.cpp UnFind.cpp Graph.cpp Graph.h MstSailman.cpp SalesmanTotal.cpp SalesmanTotal.h)