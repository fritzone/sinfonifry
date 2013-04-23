cmake_minimum_required (VERSION 2.6)

set(PROJECT 
 sinfonifry_core
)

project (${PROJECT})

set(${PROJECT}_SOURCES 
 sinfonifry_core.cpp 
 ClientThread.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)

set (
 ${PROJECT}_HEADERS
 ClientThread.h
 CACHE INTERNAL ${PROJECT}_HEADERS  
)

OPTION(CORE_PLUGIN_DISK_STATUS "Build the Disk Status plugin for Core" ON)

SET(CORE_CONFIG_BIND_IP "0.0.0.0" CACHE STRING "0.0.0.0")
SET(CORE_CONFIG_BIND_PORT "29888" CACHE STRING "29888")

SET(CORE_CONFIG_DB_HOST_IP "127.0.0.1" CACHE STRING "127.0.0.1")
SET(CORE_CONFIG_DB_USER "sinfonifry" CACHE STRING "sinfonifry")
SET(CORE_CONFIG_DB_PASS "s1nf0n1fry" CACHE STRING "s1nf0n1fry")
SET(CORE_CONFIG_DB "sinfonifry" CACHE STRING "sinfonifry")

add_subdirectory ("${SINFONIFRY_ROOT}/3rdparty/tinyxml"  "${CMAKE_CURRENT_BINARY_DIR}/tinyxml")
add_subdirectory ("${SINFONIFRY_ROOT}/3rdparty/libb64"  "${CMAKE_CURRENT_BINARY_DIR}/libb64")

add_subdirectory ("${SINFONIFRY_ROOT}/utils"  "${CMAKE_CURRENT_BINARY_DIR}/utils")
add_subdirectory ("${SINFONIFRY_ROOT}/plugins/api"  "${CMAKE_CURRENT_BINARY_DIR}/plugins/api")

include_directories("/usr/local/include/" 
  "${SINFONIFRY_ROOT}/3rdparty"
  "${SINFONIFRY_ROOT}/3rdparty/libb64"
  "${SINFONIFRY_ROOT}/utils"
  "${SINFONIFRY_ROOT}/plugins/api"
  "${SINFONIFRY_ROOT}/inc"
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file("${SINFONIFRY_ROOT}/templates/instdir.h.template" ${CMAKE_CURRENT_BINARY_DIR}/instdir.h)

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} cxxtools tntdb)

install(TARGETS ${PROJECT} DESTINATION sinfonifry/core)
install(FILES log.properties DESTINATION sinfonifry/core)
