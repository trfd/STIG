FILE(GLOB sti_include ${SOURCE_GROUP_PREFIX}/sti/include/*)
SOURCE_GROUP("sti\\include" FILES ${sti_include})
FILE(GLOB sti_include_sti ${SOURCE_GROUP_PREFIX}/sti/include/sti/*)
SOURCE_GROUP("sti\\include\\sti" FILES ${sti_include_sti})
FILE(GLOB sti_src ${SOURCE_GROUP_PREFIX}/sti/src/*)
SOURCE_GROUP("sti\\src" FILES ${sti_src})