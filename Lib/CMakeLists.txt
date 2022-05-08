project(Munkres)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)

add_library(Munkres INTERFACE)
target_link_libraries(Munkres INTERFACE Eigen3::Eigen)
target_include_directories(Munkres INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
