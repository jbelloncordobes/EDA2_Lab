cmake_minimum_required(VERSION 3.24)
project(EDA2_Lab C)

set(CMAKE_C_STANDARD 11)

add_executable(EDA2_Lab main.c sources/interface.c headers/interface.h)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -municode")