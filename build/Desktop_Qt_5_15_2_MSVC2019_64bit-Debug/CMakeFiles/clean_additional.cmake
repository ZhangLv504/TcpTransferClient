# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TcpTransferClient_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TcpTransferClient_autogen.dir\\ParseCache.txt"
  "TcpTransferClient_autogen"
  )
endif()
