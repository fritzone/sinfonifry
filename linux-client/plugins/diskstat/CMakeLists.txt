cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry_plugin_disk_status)

project(${PROJECT})

set(${PROJECT}_SOURCES
 disk_status.cpp
 utils.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)

set (
 ${PROJECT}_HEADERS ""
 utils.h
 CACHE INTERNAL ${PROJECT}_HEADERS
)

add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} blkid ext2fs)

include_directories("/usr/local/include/" "../3rdparty")

install(TARGETS ${PROJECT} LIBRARY
       DESTINATION sinfonifry/client/plugins
       NAMELINK_SKIP)
