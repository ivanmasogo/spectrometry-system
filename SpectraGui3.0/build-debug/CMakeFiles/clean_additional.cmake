# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SpectraGUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SpectraGUI_autogen.dir/ParseCache.txt"
  "SpectraGUI_autogen"
  )
endif()
