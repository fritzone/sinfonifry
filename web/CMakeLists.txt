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
)
     
set(${PROJECT}_SOURCES
  ""
  CACHE INTERNAL ${PROJECT}_SOURCES
)
     
foreach(comp ${WEB_COMPONENTS})
  list(APPEND ${PROJECT}_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp )
  execute_process(COMMAND ${ECPPC} -o ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp -v -n ${comp} ${CMAKE_CURRENT_SOURCE_DIR}/${comp}.ecpp WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
  
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp)
  
endforeach()

list(APPEND ${PROJECT}_SOURCES main.cpp )

add_executable(${PROJECT}_exe ${${PROJECT}_SOURCES})

#add_library(${PROJECT} SHARED ${${PROJECT}_SOURCES})
#set_target_properties(${PROJECT} PROPERTIES PREFIX "")

#target_link_libraries(${PROJECT}     cxxtools dl tntnet tntdb)
target_link_libraries(${PROJECT}_exe cxxtools dl tntnet tntdb)
