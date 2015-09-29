cmake_minimum_required(VERSION 3.3)
project(bignum)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    bignum_math.c
    bignum_math.h)

add_executable(bignum ${SOURCE_FILES})