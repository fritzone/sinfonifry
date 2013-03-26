cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry)

project (${PROJECT})

set (SINFONIFRY_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

OPTION(BUILD_SINFONIFRY_LINUX_CLIENT "Build the Linux Client Component")
OPTION(BUILD_SINFONIFRY_CORE "Build the Core Component")
OPTION(BUILD_SINFONIFRY_WEB "Build the Web Component")

add_subdirectory(utils)

if(BUILD_SINFONIFRY_LINUX_CLIENT)
 add_subdirectory(linux-client)
endif()

if(BUILD_SINFONIFRY_CORE)
 add_subdirectory(core)
endif()
