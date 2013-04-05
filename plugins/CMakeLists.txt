OPTION(PLUGIN_DISK_STATUS "Include the Disk Status plugin. By selecting this you will have the option to specify:\n1. To build the client plugin  (PLUGIN_DISK_STATUS_CLIENT)\n2. To build the core plugin (PLUGIN_DISK_STATUS_CORE)\n3. To build the web plugin (PLUGIN_DISK_STATUS_WEB)" OFF)

if(PLUGIN_DISK_STATUS)
  add_subdirectory(disk_status)
endif()


