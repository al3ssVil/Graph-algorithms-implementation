# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Ford-Fulkerson_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Ford-Fulkerson_autogen.dir\\ParseCache.txt"
  "Ford-Fulkerson_autogen"
  )
endif()
