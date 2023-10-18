cmake_minimum_required(VERSION 3.0)
project(ContainerAdaptorTests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_STANDARD_REQUIRED ON)

add_subdirectory(.. BagContainerAdaptorDir)

#add_executable(ContainerAdaptorTests legacy_forward_iterator_tests.cpp bag_container_adaptor_tests.cpp linked_list_tests.cpp front_and_back_tests.cpp main.cpp)
add_executable(ContainerAdaptorTests test.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(ContainerAdaptorTests BagContainerAdaptor ${GTEST_BOTH_LIBRARIES})
