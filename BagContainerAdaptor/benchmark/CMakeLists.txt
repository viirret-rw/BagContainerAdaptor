cmake_minimum_required(VERSION 3.0)

project(ContainerAdaptorBenchmark)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_STANDARD_REQUIRED ON)

add_subdirectory(.. BagContainerAdaptorDir)

add_executable(ContainerAdaptorBenchmark main.cpp)

target_link_libraries(ContainerAdaptorBenchmark BagContainerAdaptor)
