cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry_client)

project (${PROJECT})

set(${PROJECT}_SOURCES
 main.cpp
 CACHE INTERNAL ${PROJECT}_SOURCES
)

set (
 ${PROJECT}_HEADERS ""
 CACHE INTERNAL ${PROJECT}_HEADERS
)

# Options for the client
SET(CLIENT_CONFIG_CORE_HOST_IP "127.0.0.1" CACHE STRING "127.0.0.1")
SET(CLIENT_CONFIG_CORE_HOST_PORT "29888" CACHE STRING "29888")

add_subdirectory ("${SINFONIFRY_ROOT}/3rdparty/tinyxml"  "${CMAKE_CURRENT_BINARY_DIR}/tinyxml")
add_subdirectory ("${SINFONIFRY_ROOT}/utils"  "${CMAKE_CURRENT_BINARY_DIR}/utils")
add_subdirectory ("${SINFONIFRY_ROOT}/plugins/api"  "${CMAKE_CURRENT_BINARY_DIR}/plugins/api")

configure_file("${SINFONIFRY_ROOT}/templates/instdir.h.template" ${CMAKE_CURRENT_BINARY_DIR}/instdir.h)

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} cxxtools dl)

include_directories(
  "/usr/local/include/"
  "${SINFONIFRY_ROOT}/3rdparty"
  "${SINFONIFRY_ROOT}/utils"
  "${SINFONIFRY_ROOT}/plugins/api"
  "${SINFONIFRY_ROOT}/inc"
  ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS ${PROJECT} DESTINATION sinfonifry/client)
install(FILES log.properties DESTINATION sinfonifry/client)
