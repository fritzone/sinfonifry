OPTION(CORE_PLUGIN_DISK_STATUS "Build the Disk Status plugin for Core" ON)

SET(CORE_CONFIG_BIND_IP "0.0.0.0" CACHE STRING "0.0.0.0")
SET(CORE_CONFIG_BIND_PORT "29888" CACHE STRING "29888")

SET(CORE_CONFIG_DB_HOST_IP "127.0.0.1" CACHE STRING "127.0.0.1")
SET(CORE_CONFIG_DB_USER "sinfonifry" CACHE STRING "sinfonifry")
SET(CORE_CONFIG_DB_PASS "s1nf0n1fry" CACHE STRING "s1nf0n1fry")
SET(CORE_CONFIG_DB "sinfonifry" CACHE STRING "sinfonifry")

set(CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/config.xml)

# create a default configuration file which will be copied over to
# the configuration directory
FILE(WRITE  ${CONFIG_FILE} "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
FILE(APPEND ${CONFIG_FILE} "<sinfonifry>\n")

                                            
FILE(APPEND ${CONFIG_FILE} " <plugins>\n")

if(CORE_PLUGIN_DISK_STATUS)
  add_subdirectory(disk_status)
  FILE(APPEND ${CONFIG_FILE} "  <plugin name=\"disk_status\" enabled=\"1\"/>\n")
  FILE(APPEND ${CONFIG_FILE} "   <configuration />")
else()
  FILE(APPEND ${CONFIG_FILE} "  <plugin name=\"disk_status\" enabled=\"0\"/>\n")
  FILE(APPEND ${CONFIG_FILE} "   <configuration />")
endif()

FILE(APPEND ${CONFIG_FILE} " \n</plugins>\n")
 FILE(APPEND ${CONFIG_FILE} " </core>\n")
FILE(APPEND ${CONFIG_FILE} "</sinfonifry>\n")

install(FILES ${CONFIG_FILE} DESTINATION sinfonifry/core/config)
