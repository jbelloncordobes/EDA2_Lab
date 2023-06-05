cmake_minimum_required(VERSION 3.24)
project(EDA2_Lab C)

set(CMAKE_C_STANDARD 11)

add_executable(EDA2_Lab main.c sources/handleStack.c headers/handleStack.h headers/common.h sources/users.c headers/users.h sources/user_arrays.c headers/user_arrays.h sources/PostQueue.c headers/PostQueue.h sources/posts.c headers/posts.h sources/WordDictionary.c headers/WordDictionary.h)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -municode -mconsole")