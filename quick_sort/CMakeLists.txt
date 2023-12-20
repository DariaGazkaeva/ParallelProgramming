cmake_minimum_required(VERSION 3.21)
project(quick_sort)

set(CMAKE_CXX_STANDARD 14)

#-static-libstdc++

set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_CXX_FLAGS "-static-libstdc++ -static-libstdc" )

find_package(MPI REQUIRED)

add_executable(quick_sort quick_sort.cpp)
target_link_libraries(quick_sort PUBLIC MPI::MPI_CXX)
