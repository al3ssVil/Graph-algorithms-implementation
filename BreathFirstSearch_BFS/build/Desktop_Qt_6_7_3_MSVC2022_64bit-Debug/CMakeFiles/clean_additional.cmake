# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BreathFirstSearch_BFS_autogen"
  "CMakeFiles\\BreathFirstSearch_BFS_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BreathFirstSearch_BFS_autogen.dir\\ParseCache.txt"
  )
endif()
