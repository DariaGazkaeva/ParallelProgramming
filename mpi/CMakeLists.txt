cmake_minimum_required(VERSION 3.21)
project(task2_mpi)

set(CMAKE_CXX_STANDARD 14)

#-static-libstdc++

set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_CXX_FLAGS "-static-libstdc++ -static-libstdc" )

find_package(MPI REQUIRED)

add_executable(task1 task1.cpp)
target_link_libraries(task1 PUBLIC MPI::MPI_CXX)

add_executable(task2 task2.cpp)
target_link_libraries(task2 PUBLIC MPI::MPI_CXX)

add_executable(task3 task3.cpp)
target_link_libraries(task3 PUBLIC MPI::MPI_CXX)

add_executable(task4 task4.cpp)
target_link_libraries(task4 PUBLIC MPI::MPI_CXX)

add_executable(task5 task5.cpp)
target_link_libraries(task5 PUBLIC MPI::MPI_CXX)
