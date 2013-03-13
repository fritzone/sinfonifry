cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry_client)

project (${PROJECT})

set(${PROJECT}_SOURCES
 part_list.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)


set (
 ${PROJECT}_HEADERS ""
 CACHE INTERNAL ${PROJECT}_HEADERS
)

add_subdirectory ("../3rdparty/tinyxml"  "${CMAKE_CURRENT_BINARY_DIR}/tinyxml")

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} blkid ext2fs cxxtools)

include_directories("/usr/local/include/" "../3rdparty")



