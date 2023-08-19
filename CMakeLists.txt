cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project(task)

add_executable(task 
    src/main.cpp 
    src/process.cpp 
    src/funcs.cpp
    src/assert.cpp) 