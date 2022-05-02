cmake_minimum_required(VERSION 3.21)
project(Munkres)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)

add_library(Munkres SHARED src/Munkres.h src/Utility.h src/Utility.cpp)
target_link_libraries(Munkres Eigen3::Eigen)
target_include_directories(Munkres PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_compile_options(Munkres PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>)

add_executable(Test main.cpp)
target_link_libraries(Test Munkres)

