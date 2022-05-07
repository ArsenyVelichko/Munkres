project(Munkres)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)

add_library(Munkres SHARED Munkres.h Index2D.h Index2D.cpp)
target_link_libraries(Munkres Eigen3::Eigen)
target_include_directories(Munkres PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(Munkres PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>)
