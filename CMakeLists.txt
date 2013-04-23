cmake_minimum_required (VERSION 2.6)

set (PROJECT sinfonifry)

project (${PROJECT})

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
    "/opt/" CACHE PATH "sinfonifry install prefix. The application will be installed in this directory." FORCE
    )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

include(scripts/add_plugin_to_config.cmake)

set (SINFONIFRY_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (POSIX "NO")
set (BUILD_SINFONIFRY_ANY_CLIENT "NO")

# if we are on Linux switch on the posix flag
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 # set the flag
 set (POSIX "YES")
endif()

# Are we on a POSIX compliant system?
if(POSIX)

 # The Posix client can be built
 OPTION(BUILD_SINFONIFRY_POSIX_CLIENT "Build the Linux Client Component")
 
 # The Core component can be built
 OPTION(BUILD_SINFONIFRY_CORE "Build the Core Component")
 
 # The web component can be built
 OPTION(BUILD_SINFONIFRY_WEB "Build the Web Component")
 
 # Should we also install a configuration file?
 OPTION(INSTALL_CONFIG_FILE "Should we install a default configuration file based on the settings you chose?\nWARNING!!! This will NOT overwrite any older configuration files you have, to save your old setting, instead it will stay in the build (${CMAKE_CURRENT_BINARY_DIR}) directory, merge manually.")
 
 # Should we build the posix client?
 if(BUILD_SINFONIFRY_POSIX_CLIENT)
  add_subdirectory(posix-client)
  set(BUILD_SINFONIFRY_ANY_CLIENT "YES")
 endif()
 
 # Should we build the core?
 if(BUILD_SINFONIFRY_CORE)
  add_subdirectory(core)
 endif()
 
 # should we build the web component?
 if(BUILD_SINFONIFRY_WEB)
   add_subdirectory(web)
 endif()

 # by default add the utils subdirectory
 add_subdirectory(utils)

 # and the plugins
 add_subdirectory(plugins)
endif() # POSIX?

# And here create a default configuration file which will be copied over to
# the configuration directory

set(CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/config.xml)

FILE(WRITE  ${CONFIG_FILE} "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
FILE(APPEND ${CONFIG_FILE} "<sinfonifry>\n")
if(BUILD_SINFONIFRY_ANY_CLIENT)
  FILE(APPEND ${CONFIG_FILE} " <client>\n")
  FILE(APPEND ${CONFIG_FILE} " <config core-host=\"${CLIENT_CONFIG_CORE_HOST_IP}\" core-port=\"${CLIENT_CONFIG_CORE_HOST_PORT}\" sleep-time-sec=\"60\"/>\n")
  FILE(APPEND ${CONFIG_FILE} " <plugins>\n")
    
    ## here a list of the client plugins that were built
    if(PLUGIN_DISK_STATUS_CLIENT)
      add_plugin_to_config(${CONFIG_FILE} "disk_status" "1" "client")
    endif()

  FILE(APPEND ${CONFIG_FILE} " </plugins>\n")
  FILE(APPEND ${CONFIG_FILE} " </client>\n")
endif() # client

if(POSIX)
  if(BUILD_SINFONIFRY_CORE)
    FILE(APPEND ${CONFIG_FILE} " <core>\n")
    FILE(APPEND ${CONFIG_FILE} " <config bind_to=\"${CORE_CONFIG_BIND_IP}\" bind_port=\"${CORE_CONFIG_BIND_PORT}\" db_user=\"${CORE_CONFIG_DB_USER}\" db_pass=\"${CORE_CONFIG_DB_PASS}\" db_host=\"${CORE_CONFIG_DB_HOST_IP}\" db_name=\"${CORE_CONFIG_DB}\" />\n")
    FILE(APPEND ${CONFIG_FILE} " <plugins>\n")
        ## here a list of the core plugins that were built
        if(PLUGIN_DISK_STATUS_CORE)
          add_plugin_to_config(${CONFIG_FILE} "disk_status" "1" "core")
        endif()
    FILE(APPEND ${CONFIG_FILE} " </plugins>\n")
    FILE(APPEND ${CONFIG_FILE} " </core>\n")
  endif() # core
endif() # POSIX

FILE(APPEND ${CONFIG_FILE} "</sinfonifry>\n")

if(INSTALL_CONFIG_FILE)
  install(FILES ${CONFIG_FILE} DESTINATION sinfonifry/config)
endif()

