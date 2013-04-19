cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry_core_plugin_disk_status)

project(${PROJECT})

set(${PROJECT}_SOURCES
 core_plugin_disk_status.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)

set (
 ${PROJECT}_HEADERS ""
 CACHE INTERNAL ${PROJECT}_HEADERS
)

add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} blkid ext2fs)

include_directories(
  "/usr/local/include/"
  "${SINFONIFRY_ROOT}/3rdparty"
  "${SINFONIFRY_ROOT}/utils"
  "${SINFONIFRY_ROOT}/plugin-api"
  ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS ${PROJECT} LIBRARY
       DESTINATION sinfonifry/core/plugins
       NAMELINK_SKIP)
