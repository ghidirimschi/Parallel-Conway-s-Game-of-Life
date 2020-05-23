cmake_minimum_required(VERSION 3.14)
project(golMPI C)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_C_STANDARD 99)

add_executable(golMPI golMPI/main.c util/golInput.c util/golInput.h util/dish.c util/dish.h golSequential/main.c)