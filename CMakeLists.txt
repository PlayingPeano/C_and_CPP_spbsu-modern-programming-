cmake_minimum_required(VERSION 3.23)
project(labs_isaenkov)

set(CMAKE_CXX_STANDARD 17)

include_directories(lab-07_matrix/include)

add_executable(labs_isaenkov
        lab-07_matrix/include/matrix.h
        lab-07_matrix/src/main.cpp
        lab-07_matrix/src/matrix.cpp
        lab-07_matrix/tests/00-init.expected-output
        lab-07_matrix/tests/00-init.test.input
        lab-07_matrix/tests/01-get-set.expected-output
        lab-07_matrix/tests/01-get-set.test.input
        lab-07_matrix/tests/02-assign-copy.expected-output
        lab-07_matrix/tests/02-assign-copy.test.input
        lab-07_matrix/tests/03-cmp.expected-output
        lab-07_matrix/tests/03-cmp.test.input
        lab-07_matrix/README.md)
