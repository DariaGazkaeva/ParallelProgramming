cmake_minimum_required(VERSION 3.6)
project(task1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(task1 task1.cpp task2.cpp task3.cpp task4.cpp task5.cpp)
