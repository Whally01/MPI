cmake_minimum_required(VERSION 3.10)
project(mpi2)

set(CMAKE_CXX_STANDARD 11)

add_executable(mpi2 main.cpp)

set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")
set(CMAKE_C_COMPILER "/usr/bin/mpicc")

set(MPIEXEC_NUMPROC_FLAG:STRING=-n 3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -n 4)