cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(BFS VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(BFS BFS.cpp)
add_executable(Generate_BFS_Data Generate_BFS_Data.cpp)
