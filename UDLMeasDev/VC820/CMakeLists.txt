add_library (vc820 SHARED VC820.cpp ../../UDLmain/UdlSettingsFile.cpp ../../UDLlib/StringTools.cpp)
SET_TARGET_PROPERTIES(vc820 PROPERTIES SUFFIX ".md")
SET_TARGET_PROPERTIES(vc820 PROPERTIES PREFIX "udl_")