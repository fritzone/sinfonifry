macro(add_plugin_to_config CONFIG_FILE PLUGIN_NAME ENABLED)
  FILE(APPEND ${CONFIG_FILE} "  <plugin name=\"${PLUGIN_NAME}\" enabled=\"${ENABLED}\" freqeuncy=\"60\">\n")
  FILE(APPEND ${CONFIG_FILE} "   <configuration />\n  </plugin>\n")
endmacro()
 