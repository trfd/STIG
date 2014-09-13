FILE(GLOB ces_include ${SOURCE_GROUP_PREFIX}/ces/include/*)
SOURCE_GROUP("ces\\include" FILES ${ces_include})

FILE(GLOB ces_include_ces ${SOURCE_GROUP_PREFIX}/ces/include/ces/*)
SOURCE_GROUP("ces\\include\\ces" FILES ${ces_include_ces})

FILE(GLOB ces_src ${SOURCE_GROUP_PREFIX}/ces/src/*)
SOURCE_GROUP("ces\\src" FILES ${ces_src})

FILE(GLOB ces_src_swig ${SOURCE_GROUP_PREFIX}/ces/src/swig/*)
SOURCE_GROUP("ces\\src\\swig" FILES ${ces_src_swig})

