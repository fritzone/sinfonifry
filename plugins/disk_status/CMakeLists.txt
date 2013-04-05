OPTION(PLUGIN_DISK_STATUS_CLIENT "Build Disk Status plugin extension for the client" OFF)
OPTION(PLUGIN_DISK_STATUS_CORE "Build Disk Status plugin extension for the core" OFF)

if(PLUGIN_DISK_STATUS_CLIENT)
  add_subdirectory(client)
endif()  
