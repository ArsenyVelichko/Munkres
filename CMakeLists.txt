cmake_minimum_required(VERSION 3.21)
project(TestMunkres)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestMunkres main.cpp)
target_link_libraries(TestMunkres Munkres)

add_subdirectory(Lib)

