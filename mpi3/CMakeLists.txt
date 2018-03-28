cmake_minimum_required(VERSION 3.3)
project(mpi3)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)

include_directories(/usr/include/mpi/)
add_executable(mpi3 main.cpp)