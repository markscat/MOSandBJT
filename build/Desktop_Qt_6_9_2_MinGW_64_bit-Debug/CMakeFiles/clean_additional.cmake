# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MOSandBJT_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MOSandBJT_autogen.dir\\ParseCache.txt"
  "MOSandBJT_autogen"
  )
endif()
