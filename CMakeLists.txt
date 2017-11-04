cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

project(GFWX C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(include)
add_subdirectory(libgfwx)

add_subdirectory(app)
