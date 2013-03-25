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

add_subdirectory ("${SINFONIFRY_ROOT}/3rdparty/tinyxml"  "${CMAKE_CURRENT_BINARY_DIR}/tinyxml")
add_subdirectory ("${SINFONIFRY_ROOT}/utils"  "${CMAKE_CURRENT_BINARY_DIR}/utils")
add_subdirectory ("${SINFONIFRY_ROOT}/plugin-api"  "${CMAKE_CURRENT_BINARY_DIR}/plugn-api")

configure_file("${SINFONIFRY_ROOT}/templates/instdir.h.template" ${CMAKE_CURRENT_BINARY_DIR}/instdir.h)

add_subdirectory(plugins)

add_executable(${PROJECT} ${${PROJECT}_SOURCES})

target_link_libraries(${PROJECT} cxxtools dl)

include_directories(
  "/usr/local/include/"
  "${SINFONIFRY_ROOT}/3rdparty"
  "${SINFONIFRY_ROOT}/utils"
  "${SINFONIFRY_ROOT}/plugin-api"
  ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS ${PROJECT} DESTINATION sinfonifry/client)
