cmake_minimum_required(VERSION 3.7)
project(OS_myshell)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_system -lboost_filesystem")

set(SOURCE_FILES shell.cpp)

add_executable(OS_myshell shell.cpp)