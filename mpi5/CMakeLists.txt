cmake_minimum_required(VERSION 3.10)
project(mpi5)

set(CMAKE_CXX_STANDARD 11)

add_executable(mpi5 main.cpp)

include_directories(/usr/include/mpi/)

set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)