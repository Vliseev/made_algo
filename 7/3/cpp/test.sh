#!/bin/bash

mkdir -p "build"
cp gen_test.py build
cd build && cmake ../ && make

SALESMAN_PROG="$(pwd)/salesman" python3 gen_test.py