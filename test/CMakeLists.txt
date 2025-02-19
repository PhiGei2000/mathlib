cmake_minimum_required(VERSION 3.24.0)
project(mathlib_test VERSION 0.1.0 LANGUAGES C CXX)

include_directories(../include)

add_executable(mathlib_test main.cpp)
target_link_libraries(mathlib_test PRIVATE mathlib)


