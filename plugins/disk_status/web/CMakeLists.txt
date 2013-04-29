cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry_vis_plugin_disk_status)

project(${PROJECT})

set(${PROJECT}_SOURCES
 web_plugin_disk_status.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)

set (
 ${PROJECT}_HEADERS ""
 CACHE INTERNAL ${PROJECT}_HEADERS
)

add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} tntdb cxxtools)

include_directories(
  "/usr/local/include/"
  "${SINFONIFRY_ROOT}/3rdparty"
  "${SINFONIFRY_ROOT}/utils"
  "${SINFONIFRY_ROOT}/plugins/api"
  "${SINFONIFRY_ROOT}/inc"
  ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS ${PROJECT} LIBRARY
       DESTINATION sinfonifry/vis/plugins
       NAMELINK_SKIP)

get_property(${PROJECT}_location_t TARGET ${PROJECT} PROPERTY LOCATION)
set(${PROJECT}_location ${${PROJECT}_location_t} CACHE INTERNAL ${PROJECT}_location)

set(${PROJECT}_directory ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL ${PROJECT}_directory )
