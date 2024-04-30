cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(hw-02_huffman CXX)

add_executable(hw-02_huffman
        src/main.cpp
        include/huffman.h
        src/huffman.cpp
        include/bitstream.h
        src/bitstream.cpp
)

add_executable(hw-02_huffman_tests
        test/test.cpp
        include/huffman.h
        include/test.h
        src/huffman.cpp
        include/bitstream.h
        src/bitstream.cpp
)

#add_executable(asan-target
#        test/test.cpp
#        include/huffman.h
#        include/test.h
#        src/huffman.cpp
#        include/bitstream.h
#        src/bitstream.cpp
#)

target_include_directories(hw-02_huffman PUBLIC include ./* ./ ./include ./tmp/ci_6751967856/output/private)
target_include_directories(hw-02_huffman_tests PUBLIC include doctest/doctest tests)
#target_include_directories(asan-target PUBLIC include doctest/doctest tests)

target_compile_options(hw-02_huffman PRIVATE
        -Wall
        -Wextra
        -pedantic
        -Werror
#        -fsanitize=undefined
#        -fuse-ld=lld
#        -g
)

target_compile_options(hw-02_huffman_tests PRIVATE
        -Wall
        -Wextra
        -pedantic
        -Werror
#        -fsanitize=undefined
#        -fuse-ld=lld
#        -g
)

#target_compile_options(asan-target PRIVATE -fsanitize=address)
#target_link_options(asan-target PRIVATE -fsanitize=address)
