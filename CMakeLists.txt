cmake_minimum_required(VERSION 3.7)
project(OS_myshell)
set(BOOST_ROOT "/usr/local/Cellar/boost/1.66.0")
find_package(Boost 1.66.0 COMPONENTS system filesystem REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES shell.cpp)

add_executable(OS_myshell shell.cpp)
