cmake_minimum_required(VERSION 4.0)
project(DA_project1)

set(CMAKE_CXX_STANDARD 17)

add_executable(DA_project1 src/main.cpp
        src/parser.cpp
        src/parser.h
        src/dataStructures.h
        src/graph.h
        src/createGraph.h
        src/createGraph.cpp)
