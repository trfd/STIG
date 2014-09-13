IF(CMEXT_INCLUDED)
	MESSAGE("CMExt already included")
ELSE()
	SET(CMEXT_INCLUDED "true")
ENDIF()

FUNCTION(cmext_find_lib _libname _directory)
	SET(tmp_files)
	FILE(GLOB_RECURSE 
		 tmp_files
		 ${_directory}
		 "(lib)?(boost_test).*\.(so|dylib|lib|a|dll)$"
		 )

	IF(tmp_files)
		
	ELSE()
		MESSAGE("Library ${_libname} not found at ${_directory}")
	ENDIF()

ENDFUNCTION(cmext_find_lib)

FUNCTION(print_files _files)
	FOREACH(file ${_files})
		MESSAGE("-${file}")
	ENDFOREACH()
ENDFUNCTION()

### Defines the variable <_name>_STATIC_LIB_NAME
### with the full name of library according to current
### OS (Name.lib / libName.a)
MACRO(static_lib_name _name)
	IF(WIN32)
		SET(${_name}_STATIC_LIB_NAME ${_name}.lib)
	ELSE()
		SET(${_name}_STATIC_LIB_NAME lib${_name}.a)
	ENDIF()
ENDMACRO(static_lib_name)


### Defines the variable <_name>_DYNAMIC_LIB_NAME
### with the full name of library according to current
### OS (Name.dll / libName.dylib / libName.so)
MACRO(dynamic_lib_name _name)
	IF(WIN32)
		SET(${_name}_DYNAMIC_LIB_NAME ${_name}.dll)
	ELSEIF(APPLE)
		SET(${_name}_DYNAMIC_LIB_NAME lib${_name}.dylib)
	ELSE()
		SET(${_name}_DYNAMIC_LIB_NAME lib${_name}.so)
	ENDIF()
ENDMACRO(dynamic_lib_name)

### Check if variable _var is set
### if not _message is displayed
### as FATAL_ERROR
MACRO(cmext_require _var _message)
	IF(NOT ${_var})
		MESSAGE(FATAL_ERROR ${_message})
	ENDIF()
ENDMACRO(cmext_require)


### USAGE : cmext_add_module(_name [PATH _location])
### Basically include another CMake File
### with a specific context:
### Arguments: 
### _name     : Name of the module
### _location : Relative path of module file (relative to CMAKE_SOURCE_DIR)
### Defines the following vars: 
### <_name>_INCLUDED 
### <_name>_ROOT_DIR_GLOBAL : The global path of directory containing module file
### <_name>_MODULE_FILE	    : The name of the module file
### Appends INCLUDE_DIRS with <_name>_INCLUDE_DIR
### Apply source grouping if source grouping script is found
###
MACRO(cmext_add_module _name)

	IF(${ARGC} EQUAL 1)
	
		SET(${_name}_ROOT_DIR_GLOBAL ${CMAKE_SOURCE_DIR})
		SET(${_name}_MODULE_FILE  ${CMAKE_SOURCE_DIR}/${_name}.cmake)
	
	ELSEIF(${ARGC} EQUAL 3 AND ${ARGV1} STREQUAL "PATH")
	
		SET(${_name}_ROOT_DIR_GLOBAL ${CMAKE_SOURCE_DIR}/${ARGV2})
		SET(${_name}_MODULE_FILE ${${_name}_ROOT_DIR_GLOBAL}/${_name}.cmake)
	
	ELSE()
		MESSAGE(FATAL_ERROR "Syntax Error with cmext_add_module")
	ENDIF()

	IF(EXISTS ${${_name}_MODULE_FILE})

  		INCLUDE(${${_name}_MODULE_FILE})
  		
  		SET(${_name}_INCLUDED "true")
  		
		MESSAGE("Module ${_name} added")

  		# Include directories
		IF(${_name}_INCLUDE_DIR)
			SET(INCLUDE_DIRS ${INCLUDE_DIRS} ${${_name}_INCLUDE_DIR})
		ELSE()
			MESSAGE("***WARNING: Module ${_name} does not define ${_name}_INCLUDE_DIR")
		ENDIF()

		FOREACH(_target ${${_name}_TARGETS})
			SET_PROPERTY(TARGET ${_target}
					 PROPERTY INCLUDE_DIRECTORIES
					 ${INCLUDE_DIRS}
					 )
		ENDFOREACH()
  		
	ELSE()
		MESSAGE("Module File ${${name}_MODULE_FILE} not found")
	ENDIF()

ENDMACRO(cmext_add_module)


### Apply Source Grouping to a module _module
MACRO(cmext_source_group _module _root)

	SET(SOURCE_GROUP_PREFIX ${${_module}_ROOT_DIR_GLOBAL})

	SET(_SRC_GRP_OUT_FILE ${${_module}_ROOT_DIR_GLOBAL}/cmake_config/src_group_${_module}.cmake)

	IF(EXISTS ${${_module}_ROOT_DIR_GLOBAL}/src_grp)
		MESSAGE("=====================================================")
		MESSAGE(" | Command: ")
		EXECUTE_PROCESS(
			COMMAND ${${_module}_ROOT_DIR_GLOBAL}/src_grp ${_root} ${_SRC_GRP_OUT_FILE}
			)
		MESSAGE("=====================================================")
	ENDIF()
	
	IF(EXISTS ${_SRC_GRP_OUT_FILE})
		INCLUDE(${_SRC_GRP_OUT_FILE})
		MESSAGE("Apply source grouping with file found at ${_SRC_GRP_OUT_FILE}")
	ELSE()
		MESSAGE("***WARNING: No source grouping file found at ${_SRC_GRP_OUT_FILE}")
	ENDIF()
ENDMACRO(cmext_source_group)


### Add a file directory to a module
### This basically fills the variable
### <_name>_FILES with the content of 
### directory _filedir.
MACRO(cmext_add_file_dir _module _name _filedir)
	
	FILE(	
		GLOB_RECURSE 
		tmp_files 
		${${_module}_ROOT_DIR_GLOBAL}/${_filedir}/*
	)

	IF(${_name}_FILES)
		SET(${_name}_FILES ${${_name}_FILES} ${tmp_files})
	ELSE()
		SET(${_name}_FILES ${tmp_files})
	ENDIF()

ENDMACRO(cmext_add_file_dir)

### Add a library target to a module and link to libs.
### The source files used are <_name>_FILES.
### The links used are <_name>_LINK_LIBS.
### _libType is the type of library (static, dynamic,...)
### Usage cmext_add_library(LibName [STATIC|SHARED...])
MACRO(cmext_add_library _module _name _libType)

	IF(NOT ${_name}_FILES)
		MESSAGE(FATAL_ERROR "Variable ${_name}_FILES not set before add library")
	ENDIF()

	ADD_LIBRARY(
		${_name} ${_libType}
		${${_name}_FILES}
	)

	IF(${_name}_LINK_LIBS)
		
		TARGET_LINK_LIBRARIES(
			${_name}
			${${_name}_LINK_LIBS}
		)

	ELSE()
		MESSAGE("***WARNING: No library linked to ${_name} ( variable ${_name}_LINK_LIBS is not set)")
	ENDIF()

	SET_TARGET_PROPERTIES(${_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY         ${LIB_DIR})
	SET_TARGET_PROPERTIES(${_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${LIB_DIR})
	SET_TARGET_PROPERTIES(${_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_DIR})

	SET(${_module}_TARGETS ${${_module}_TARGETS} ${_name})

ENDMACRO(cmext_add_library)

### Add an executable target to a module and link to libs.
### The source files used are <_name>_FILES.
### The links used are <_name>_LINK_LIBS.
### Usage cmext_add_executable(LibName)
MACRO(cmext_add_executable _module _name)

	IF(NOT ${_name}_FILES)
		MESSAGE(FATAL_ERROR "Variable ${_name}_FILES not set before add executable")
	ENDIF()

	ADD_EXECUTABLE(
		${_name}
		${${_name}_FILES}
	)

	IF(${_name}_LINK_LIBS)
		
		TARGET_LINK_LIBRARIES(
			${_name}
			${${_name}_LINK_LIBS}
		)

	ELSE()
		MESSAGE("***WARNING: No library linked to ${_name} ( variable ${_name}_LINK_LIBS is not set)")
	ENDIF()

	SET_TARGET_PROPERTIES(${_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY         ${BIN_DIR})
	SET_TARGET_PROPERTIES(${_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${BIN_DIR})
	SET_TARGET_PROPERTIES(${_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})

	SET(${_module}_TARGETS ${${_module}_TARGETS} ${_name})

ENDMACRO(cmext_add_executable)

### Add unit test executable target to a module and link to libs.
### The module source files used are <_name>_FILES and
### the unit test sources are found in directory _unit_dir.
### The links used are <_name>_LINK_LIBS and UNIT_TEST_FRAMEWORK_LIBRARIES
### Usage cmext_add_executable(LibName)
MACRO(cmext_add_unit_test _module _target _unit_dir)

	IF(NOT UNIT_TEST_FRAMEWORK_LIBRARIES OR NOT UNIT_TEST_FRAMEWORK_INCLUDE_DIR) 
    	MESSAGE(FATAL_ERROR "Unit Test framework not defined")
	ENDIF()


	FILE(	
		GLOB_RECURSE 
		${_name}_UNIT_FILES 
		${${_module}_ROOT_DIR_GLOBAL}/${_unit_dir}/*
	)

	ADD_EXECUTABLE(
		${_name}-Unit
		${${_name}_UNIT_FILES}
		${${_name}_FILES}
	)
		
	TARGET_LINK_LIBRARIES(
		${_name}-Unit
		${${_name}_LINK_LIBS}
		${UNIT_TEST_FRAMEWORK_LIBRARIES}
	)

	SOURCE_GROUP("${_name}\\unit" FILES ${${_name}_UNIT_FILES})

ENDMACRO(cmext_add_unit_test)
