# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CS499_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CS499_autogen.dir\\ParseCache.txt"
  "CS499_autogen"
  )
endif()
