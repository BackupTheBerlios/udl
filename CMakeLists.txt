# The name of our project is "UDL"
# refer to the root source directory of the project as ${UDL_SOURCE_DIR} and
# to the root binary directory of the project as ${UDL_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (UDL)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/devices)


# Recurse into the subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (UDLmain)

add_subdirectory (UDLMeasDev/Test) 
add_subdirectory (UDLMeasDev/VC820) 