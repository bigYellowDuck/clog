cmake_minimum_required(VERSION 3.1)

project(testLogStream)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

set(CMAKE_CXX_COMPLIER "clang++")

aux_source_directory(. DIR_SRCS)

add_executable(testLogStream ${DIR_SRCS})
