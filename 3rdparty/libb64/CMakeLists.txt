set (LIBB64_SOURCES
 ${CMAKE_CURRENT_SOURCE_DIR}/b64.cpp
)

set (LIBB64_HEADERS
 ${CMAKE_CURRENT_SOURCE_DIR}/b64.h
)

list(APPEND ${PROJECT}_SOURCES ${LIBB64_SOURCES} )
list(APPEND ${PROJECT}_HEADERS ${LIBB64_HEADERS} )

set(${PROJECT}_SOURCES ${${PROJECT}_SOURCES} CACHE INTERNAL ${PROJECT}_SOURCES)
