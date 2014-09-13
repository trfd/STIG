FILE(GLOB stig_include ${SOURCE_GROUP_PREFIX}/stig/include/*)
SOURCE_GROUP("stig\\include" FILES ${stig_include})

FILE(GLOB stig_include_stig ${SOURCE_GROUP_PREFIX}/stig/include/stig/*)
SOURCE_GROUP("stig\\include\\stig" FILES ${stig_include_stig})

FILE(GLOB stig_src ${SOURCE_GROUP_PREFIX}/stig/src/*)
SOURCE_GROUP("stig\\src" FILES ${stig_src})

