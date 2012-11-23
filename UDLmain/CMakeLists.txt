# Make sure the compiler can find include files from our Hello library.
#include_directories (${UDL_SOURCE_DIR}/UDLmain)

# Make sure the linker can find the Hello library once it is built.
#link_directories (${UDL_BINARY_DIR}/UDL)


# Add executable called "udl" that is built from the source files.
# The extensions are automatically found.
add_executable (udl ../UDLlib/UdlSystem.cpp ../UDLlib/DynamicLib.cpp ../UDLlib/StringTools.cpp ../UDLlib/getopt_pp/getopt_pp.cpp UDLMain.cpp UDLTask.cpp UDLDataBase.cpp UdlDbCsv.cpp UDLDevices.cpp UDLDevice.cpp UDLMeasDevice.cpp UdlSettings.cpp UdlStdOut.cpp UdlSettingsFile.cpp)

find_library (LIBSTDCXX NAMES stdc++)
IF(UNIX)
target_link_libraries(udl -lrt)
target_link_libraries(udl -ldl)
ENDIF()
