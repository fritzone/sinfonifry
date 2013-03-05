cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry_core)

project (${PROJECT})

add_executable(${PROJECT} sinfonifry_core.cpp)

target_link_libraries(${PROJECT} cxxtools)

include_directories("/usr/local/include/")



