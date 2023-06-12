cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(zmp VERSION 1.0)

set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")

add_library(zmp zmp/zmp.c)
target_include_directories(zmp PUBLIC ${PROJECT_SOURCE_DIR})

add_subdirectory(bench)