cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry)

project (${PROJECT})

add_executable(${PROJECT} part_list.cpp)

target_link_libraries(${PROJECT} blkid ext2fs)



