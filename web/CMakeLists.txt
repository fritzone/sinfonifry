cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry)

set (ECPPC /usr/local/bin/ecppc)

set (WEB_COMPONENTS 
     sinfonifry 
     images 
     add_host
     get_data
     get_host_status
     jscript
     menu
     css
     host_list
     host_info
     dashboard
)
     
set(${PROJECT}_SOURCES
  ""
  CACHE INTERNAL ${PROJECT}_SOURCES
)

OPTION(BUILD_EXECUTABLE_WEB_COMPONENT "Build a standalone executable of the web component. If you don't select this, a library will be built, you will need to run it with tntnet." OFF)

add_subdirectory ("${SINFONIFRY_ROOT}/3rdparty/libb64"  "${CMAKE_CURRENT_BINARY_DIR}/libb64")
add_subdirectory ("${SINFONIFRY_ROOT}/3rdparty/tinyxml"  "${CMAKE_CURRENT_BINARY_DIR}/tinyxml")
add_subdirectory ("${SINFONIFRY_ROOT}/plugins/api"  "${CMAKE_CURRENT_BINARY_DIR}/plugins/api")
add_subdirectory ("${SINFONIFRY_ROOT}/utils"  "${CMAKE_CURRENT_BINARY_DIR}/utils")

foreach(comp ${WEB_COMPONENTS})
  list(APPEND ${PROJECT}_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp )
  #execute_process(COMMAND ${ECPPC} -o ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp -v -n ${comp} ${CMAKE_CURRENT_SOURCE_DIR}/${comp}.ecpp WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
  
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp
                     COMMAND ${ECPPC} -o ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp -v -n ${comp} ${CMAKE_CURRENT_SOURCE_DIR}/${comp}.ecpp
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${comp}.ecpp)
  
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp)
  
endforeach()

configure_file("${SINFONIFRY_ROOT}/templates/instdir.h.template" ${CMAKE_CURRENT_BINARY_DIR}/instdir.h)

list(APPEND ${PROJECT}_SOURCES main.cpp )

include_directories(
  "/usr/local/include/"
  "${SINFONIFRY_ROOT}/3rdparty"
  "${SINFONIFRY_ROOT}/utils"
  "${SINFONIFRY_ROOT}/plugins/api"
  "${SINFONIFRY_ROOT}/inc"
  ${CMAKE_CURRENT_BINARY_DIR}
)

MESSAGE( ${${PROJECT}_SOURCES})

if(BUILD_EXECUTABLE_WEB_COMPONENT)
  add_executable(${PROJECT}_exe ${${PROJECT}_SOURCES})
  target_link_libraries(${PROJECT}_exe cxxtools dl tntnet tntdb)
else()
  add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})
  set_target_properties(${PROJECT} PROPERTIES PREFIX "")
  target_link_libraries(${PROJECT}     cxxtools dl tntnet tntdb)
endif()
