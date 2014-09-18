FILE(GLOB STI_include ${SOURCE_GROUP_PREFIX}/STI/include/*)
SOURCE_GROUP("STI\\include" FILES ${STI_include})

FILE(GLOB STI_include_sti ${SOURCE_GROUP_PREFIX}/STI/include/sti/*)
SOURCE_GROUP("STI\\include\\sti" FILES ${STI_include_sti})

FILE(GLOB STI_src ${SOURCE_GROUP_PREFIX}/STI/src/*)
SOURCE_GROUP("STI\\src" FILES ${STI_src})

