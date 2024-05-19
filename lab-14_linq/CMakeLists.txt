cmake_minimum_required(VERSION 3.22.1)
project(lab_14_linq)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)
include_directories(src)

add_executable(lab_14_linq_test
        include/linq.hpp
        src/minimal_smoke_test.cpp
        src/smoke_test.cpp)
