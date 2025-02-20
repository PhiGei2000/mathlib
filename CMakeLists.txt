cmake_minimum_required(VERSION 3.23.0)
project(mathlib VERSION 0.1.0 LANGUAGES C CXX)

if (WIN32)
add_compile_definitions(WIN32)
endif()

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

include(sourcelist.cmake)

add_library(mathlib ${SOURCES})

add_subdirectory(test)
