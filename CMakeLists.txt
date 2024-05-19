cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project(task)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task 
    src/main.cpp 
    src/process.cpp 
    src/funcs.cpp
    src/assert.cpp) 