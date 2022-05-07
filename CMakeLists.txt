cmake_minimum_required(VERSION 3.21)
project(TestMunkres)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 3)

if (Catch2_FOUND)
    add_executable(TestMunkres test.cpp)
    target_link_libraries(TestMunkres PRIVATE Catch2::Catch2WithMain Munkres)
else()
    message(WARNING "Tests disabled because Catch2 was not found")
endif()

add_subdirectory(Lib)

