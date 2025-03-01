# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\KruskalAlgorithm_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\KruskalAlgorithm_autogen.dir\\ParseCache.txt"
  "KruskalAlgorithm_autogen"
  )
endif()
