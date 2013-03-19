OPTION(DISK_STATUS_PLUGIN "Build the Disk Status plugin." ON)
SET(CORE_HOST_IP "127.0.0.1" CACHE STRING "127.0.0.1")
SET(CORE_HOST_PORT "29888" CACHE STRING "29888")

set(CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/config.xml)

# create a default configuration file which will be copied over to
# the configuration directory
FILE(WRITE  ${CONFIG_FILE} "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
FILE(APPEND ${CONFIG_FILE} "<sinfonifry>\n")
FILE(APPEND ${CONFIG_FILE} " <client>\n")
FILE(APPEND ${CONFIG_FILE} " <config core-host=\"${CORE_HOST_IP}\" core-port=\"${CORE_HOST_PORT}\" sleep-time-sec=\"60\"/>\n")
FILE(APPEND ${CONFIG_FILE} " <plugins>\n")

if(DISK_STATUS_PLUGIN)
  add_subdirectory(diskstat)
  FILE(APPEND ${CONFIG_FILE} "  <plugin name=\"disk_status\" enabled=\"1\" freqeuncy=\"60\"/>\n")
else()
  FILE(APPEND ${CONFIG_FILE} "  <plugin name=\"disk_status\" enabled=\"0\" freqeuncy=\"60\"/>\n")
endif()

FILE(APPEND ${CONFIG_FILE} " </plugins>\n")
 FILE(APPEND ${CONFIG_FILE} " </client>\n")
FILE(APPEND ${CONFIG_FILE} "</sinfonifry>\n")

install(FILES ${CONFIG_FILE} DESTINATION sinfonifry/client/config)

