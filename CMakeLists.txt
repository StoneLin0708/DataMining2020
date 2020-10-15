cmake_minimum_required(VERSION 3.0.0)

project(fpg)

set(CXX_STANDARD 17)

include_directories(combinations)
add_subdirectory(fmt EXCLUDE_FROM_ALL)

add_subdirectory(src)
