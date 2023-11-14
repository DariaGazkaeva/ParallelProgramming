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

add_executable(task2_1 task2_1.cpp)
target_link_libraries(task2_1 PUBLIC MPI::MPI_CXX)

add_executable(task2_2 task2_2.cpp)
target_link_libraries(task2_2 PUBLIC MPI::MPI_CXX)

add_executable(task2_3 task2_3.cpp)
target_link_libraries(task2_3 PUBLIC MPI::MPI_CXX)

add_executable(task2_4 task2_4.cpp)
target_link_libraries(task2_4 PUBLIC MPI::MPI_CXX)

add_executable(task2_5 task2_5.cpp)
target_link_libraries(task2_5 PUBLIC MPI::MPI_CXX)

add_executable(task3_1 task3_1.cpp)
target_link_libraries(task3_1 PUBLIC MPI::MPI_CXX)

add_executable(task3_2 task3_2.cpp)
target_link_libraries(task3_2 PUBLIC MPI::MPI_CXX)

add_executable(task3_3 task3_3.cpp)
target_link_libraries(task3_3 PUBLIC MPI::MPI_CXX)

add_executable(task3_4 task3_4.cpp)
target_link_libraries(task3_4 PUBLIC MPI::MPI_CXX)

add_executable(task3_5 task3_5.cpp)
target_link_libraries(task3_5 PUBLIC MPI::MPI_CXX)
