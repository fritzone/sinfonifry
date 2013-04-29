OPTION(PLUGIN_DISK_STATUS_CLIENT "Build Disk Status plugin extension for the client" OFF)
OPTION(PLUGIN_DISK_STATUS_CORE "Build Disk Status plugin extension for the core" OFF)
OPTION(PLUGIN_DISK_STATUS_WEB "Build Disk Status plugin extension for the web" OFF)

if(PLUGIN_DISK_STATUS_CLIENT)
  add_subdirectory(client)
endif()

if(PLUGIN_DISK_STATUS_CORE)
  add_subdirectory(core)
endif()

if(PLUGIN_DISK_STATUS_WEB)
  add_subdirectory(web)
endif()
