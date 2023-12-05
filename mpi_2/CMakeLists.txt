cmake_minimum_required(VERSION 3.21)
project(mpi_2)

set(CMAKE_CXX_STANDARD 14)

#-static-libstdc++

set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_CXX_FLAGS "-static-libstdc++ -static-libstdc" )

find_package(MPI REQUIRED)

add_executable(task4_1 task4_1.cpp)
target_link_libraries(task4_1 PUBLIC MPI::MPI_CXX)

add_executable(task4_2 task4_2.cpp)
target_link_libraries(task4_2 PUBLIC MPI::MPI_CXX)

#add_executable(task5_1 task5_1.cpp)
#target_link_libraries(task5_1 PUBLIC MPI::MPI_CXX)
#
#add_executable(task5_2 task5_2.cpp)
#target_link_libraries(task5_2 PUBLIC MPI::MPI_CXX)
#
#add_executable(task5_3 task5_3.cpp)
#target_link_libraries(task5_3 PUBLIC MPI::MPI_CXX)
