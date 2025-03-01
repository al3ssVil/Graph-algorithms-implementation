# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DijkstraAlgorithm_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DijkstraAlgorithm_autogen.dir\\ParseCache.txt"
  "DijkstraAlgorithm_autogen"
  )
endif()
