cmake_minimum_required(VERSION 3.0)
project(BagContainerAdaptor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(SOURCES src/library.cpp)

add_compile_options(-Wall -Wextra -Werror -Wconversion -pedantic)
add_library(BagContainerAdaptor ${SOURCES})
target_include_directories(BagContainerAdaptor PUBLIC include)
