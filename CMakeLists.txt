cmake_minimum_required(VERSION 3.17)
project(COP_3530_Final_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COP_3530_Final_Project
        Graph.cpp
        main.cpp
        Movie.cpp
        Movie.h)
