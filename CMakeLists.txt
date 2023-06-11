cmake_minimum_required(VERSION 3.24)


set(CMAKE_C_STANDARD 11)

add_executable(PLANB main.c headers/common.h sources/users.c headers/users.h sources/csvusers.c sources/csvfriends.c headers/csvfriends.h)
