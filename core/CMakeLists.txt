cmake_minimum_required (VERSION 2.6)

set(PROJECT 
 sinfonifry_core
)

project (${PROJECT})

set(${PROJECT}_SOURCES 
 sinfonifry_core.cpp 
 client_thread.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)

set (
 ${PROJECT}_HEADERS
client_thread.h
 CACHE INTERNAL ${PROJECT}_HEADERS  
)

add_subdirectory ("../3rdparty/tinyxml"  "${CMAKE_CURRENT_BINARY_DIR}/tinyxml")

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} cxxtools tntdb)

include_directories("/usr/local/include/" "../3rdparty")
