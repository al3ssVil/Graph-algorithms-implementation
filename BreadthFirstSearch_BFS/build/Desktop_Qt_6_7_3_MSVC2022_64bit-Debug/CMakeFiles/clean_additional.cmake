# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BreadthFirstSearch_BFS_autogen"
  "CMakeFiles\\BreadthFirstSearch_BFS_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BreadthFirstSearch_BFS_autogen.dir\\ParseCache.txt"
  )
endif()
