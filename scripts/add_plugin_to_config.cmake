macro(add_plugin_to_config CONFIG_FILE PLUGIN_NAME ENABLED COMP)
  FILE(APPEND ${CONFIG_FILE} "  <plugin name=\"${PLUGIN_NAME}\" enabled=\"${ENABLED}\" freqeuncy=\"60\"")
  get_filename_component(lib_name ${sinfonifry_${COMP}_plugin_${PLUGIN_NAME}_location} NAME)
  FILE(APPEND ${CONFIG_FILE} "") # append more stuff if required
  FILE(APPEND ${CONFIG_FILE} ">\n")
  FILE(APPEND ${CONFIG_FILE} "   <config library=\"${lib_name}\" provides_signature=\"1\" signature = \"disk_status_una_frog\" provides_name=\"1\" />\n  </plugin>\n")
endmacro()
 