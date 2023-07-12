cmake_minimum_required(VERSION 3.0)

project(ContainerAdaptor)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Werror -Wconversion -pedantic)

add_executable(ContainerAdaptor main.cpp)
