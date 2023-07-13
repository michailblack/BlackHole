cmake_minimum_required(VERSION 3.25)

project(stb_image CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

add_library(${PROJECT_NAME} stb_image.h stb_image.cpp)
