# This will inject a plugins' SQL creation script into the SQL script of the core

macro(inject_plugin_to_sql_script plugin_name)
  set(DIRV_NAME "sinfonifry_core_plugin_${plugin_name}_directory")
  FILE(READ ${${DIRV_NAME}}/${plugin_name}.sql lines)
  FILE(APPEND ${CMAKE_BINARY_DIR}/create_db.sql "${lines}") # need the quotes in order to save the semicolons :) CMake rulez :D
endmacro()
