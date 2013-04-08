set (PLUGIN_API_SOURCES
)

set (PLUGIN_API_HEADERS
  sinfonifry_plugin_base.h
  sinfonifry_plugin_client.h
  sinfonifry_plugin_core.h
  sinfonifry_signed_plugin.h
)

list(APPEND ${PROJECT}_SOURCES ${PLUGIN_API_SOURCES} )
list(APPEND ${PROJECT}_HEADERS ${PLUGIN_API_HEADERS} )

set(${PROJECT}_SOURCES ${${PROJECT}_SOURCES} CACHE INTERNAL ${PROJECT}_SOURCES)

