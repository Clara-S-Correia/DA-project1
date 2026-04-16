cmake_minimum_required(VERSION 4.0)
project(DA2026_PRJ1_T06_G03)

set(CMAKE_CXX_STANDARD 17)

add_executable(DA2026_PRJ1_T06_G03 src/main.cpp
        src/parser.cpp
        src/parser.h
        src/dataStructures.h
        src/graph.h
        src/createGraph.h
        src/createGraph.cpp
        src/code.cpp
        src/algorithm.h)
