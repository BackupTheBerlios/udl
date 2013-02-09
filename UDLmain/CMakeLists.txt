
# Add executable called "udl" that is built from the source files.
# The extensions are automatically found.
add_executable (udl ../UDLlib/UdlSystem.cpp ../UDLlib/DynamicLib.cpp ../UDLlib/StringTools.cpp ../UDLlib/getopt_pp/getopt_pp.cpp UDLMain.cpp UDLTask.cpp UDLDataBase.cpp UdlDbCsv.cpp UDLDevices.cpp UDLDevice.cpp UDLMeasDevice.cpp UdlSettings.cpp UdlStdOut.cpp UdlSettingsFile.cpp)

add_library(tinyxml2 STATIC ../lib/tinyxml2/tinyxml2.cpp)
target_link_libraries(udl tinyxml2)

find_library (LIBSTDCXX NAMES stdc++)
IF(UNIX)
target_link_libraries(udl -lrt)
target_link_libraries(udl -ldl)
ENDIF()
