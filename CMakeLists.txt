cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0091 NEW)
project(MafiaMP CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include_directories(${PROJECT_SOURCE_DIR}/code/framework/src/)

add_subdirectory(code)
