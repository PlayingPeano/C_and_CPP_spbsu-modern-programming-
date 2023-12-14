cmake_minimum_required(VERSION 3.23)
project(lab_04_intrusive_list C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab_04_intrusive_list
        src/clist.c
        include/clist.h
        src/main.c)
