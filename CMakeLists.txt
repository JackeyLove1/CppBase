cmake_minimum_required(VERSION 3.24.2)
PROJECT(CppBase)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -O2 -pthread")

list(APPEND SRCS "defer.h"
        "macros.h"
        "status.h")
message("SRCS " ${SRCS})
# test
add_executable(defer_test defer_test.cpp ${SRCS})
