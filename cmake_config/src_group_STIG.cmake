FILE(GLOB STIG_include ${SOURCE_GROUP_PREFIX}/STIG/include/*)
SOURCE_GROUP("STIG\\include" FILES ${STIG_include})

FILE(GLOB STIG_include_stig ${SOURCE_GROUP_PREFIX}/STIG/include/stig/*)
SOURCE_GROUP("STIG\\include\\stig" FILES ${STIG_include_stig})

FILE(GLOB STIG_include_stig_ces ${SOURCE_GROUP_PREFIX}/STIG/include/stig/ces/*)
SOURCE_GROUP("STIG\\include\\stig\\ces" FILES ${STIG_include_stig_ces})

FILE(GLOB STIG_src ${SOURCE_GROUP_PREFIX}/STIG/src/*)
SOURCE_GROUP("STIG\\src" FILES ${STIG_src})

FILE(GLOB STIG_src_swig ${SOURCE_GROUP_PREFIX}/STIG/src/swig/*)
SOURCE_GROUP("STIG\\src\\swig" FILES ${STIG_src_swig})

