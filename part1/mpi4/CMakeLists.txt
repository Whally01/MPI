cmake_minimum_required(VERSION 3.9)
project(mpi4)

set(CMAKE_CXX_STANDARD 11)

add_executable(mpi4 main.cpp)

include_directories(/usr/include/mpi/)

set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)