cmake_minimum_required(VERSION 3.1)

project(testLog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

set(CMAKE_CXX_COMPLIER "clang++")

aux_source_directory(. DIR_SRCS)

add_executable(testLog ${DIR_SRCS})

target_link_libraries(testLog pthread)
