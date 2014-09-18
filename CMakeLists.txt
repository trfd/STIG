CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

###
## Append Module Path
### 
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_config")


###
## Load CMExt 
### 

IF(EXISTS ${CMAKE_SOURCE_DIR}/cmake_config/CMExt.cmake)
	INCLUDE(${CMAKE_SOURCE_DIR}/cmake_config/CMExt.cmake)
ELSE()
	MESSAGE(FATAL_ERROR "CMake Extention file (cmake_config/CMExt.cmake) not found ")
ENDIF()

IF(NOT CMEXT_INCLUDED)
	MESSAGE(FATAL_ERROR "cmake_config/CMExt.cmake not included and required")
ENDIF()

###
## Project
###

PROJECT(STIG)

###
## Libraries
###

FIND_PACKAGE(Lua REQUIRED)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE( Boost COMPONENTS system filesystem REQUIRED )
#ENABLE_TESTING()
FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES()

INCLUDE(${CMAKE_SOURCE_DIR}/ClangTool.cmake)

MESSAGE("Boost libraries: ${Boost_LIBRARIES}")
SET(CMAKE_SWIG_FLAGS "")

###
## Compilation Flags
###

set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY           "libc++")
set(CMAKE_CXX_FLAGS                                   "${CMAKE_CXX_FLAGS} -std=c++0x")

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")
ENDIF(MSVC)

## Disable RTTI
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} /GR-")
ELSE()
	MESSAGE(FATAL_ERROR "Unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
ENDIF()

###
## Configure Files
###

CONFIGURE_FILE(cmake_config/dev_paths.hpp ${CMAKE_SOURCE_DIR}/devt/source/)
CONFIGURE_FILE(cmake_config/unit_paths.hpp ${CMAKE_SOURCE_DIR}/unit_test/source/)

###
## Outputs
###

SET(BIN_DIR ${CMAKE_SOURCE_DIR}/build/bin)
SET(LIB_DIR ${CMAKE_SOURCE_DIR}/build/lib)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY         ${LIB_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${LIB_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_DIR})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${BIN_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${BIN_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})

###
## Setup
###

CMEXT_REQUIRE(Clang_INCLUDE_DIRS "STIG requires Clang_INCLUDE_DIRS")

SET(INCLUDE_DIRS 
	${Boost_INCLUDE_DIRS}
	${Clang_INCLUDE_DIRS}
	${GTEST_INCLUDE_DIRS}
	${LUA_INCLUDE_DIR})

###
## Modules
###

IF(EXISTS ${CMAKE_SOURCE_DIR}/CytokUtils)
	CMEXT_ADD_MODULE(CytokUtils PATH CytokUtils)
ELSE()
	SET(CytokUtils_INCLUDE_DIR "" CACHE PATH "## STIG: Root Directory containing headers of CytokUtils")
	SET(CytokUtils_LIBRARIES   "" CACHE PATH "## STIG: Path of cAttrib lib: libckutils.a/ckutils.lib")
ENDIF()

## Add Module cAttrib
IF(EXISTS ${CMAKE_SOURCE_DIR}/cAttrib)
	CMEXT_ADD_MODULE(cAttrib PATH cAttrib)
ELSE()
	SET(CAttrib_INCLUDE_DIR "" CACHE PATH "## STIG: Root Directory containing headers of cAttrib")
	SET(CAttrib_LIBRARIES   "" CACHE PATH "## STIG: Path of cAttrib lib: libcAttrib.a/cAttrib.lib")
ENDIF()

## Add Module CES
IF(EXISTS ${CMAKE_SOURCE_DIR}/CES)
	CMEXT_ADD_MODULE(CES PATH CES)
ELSE()
	SET(CES_INCLUDE_DIR "" CACHE PATH "## STIG: Root Directory containing headers of CES")
	SET(CES_LIBRARIES   "" CACHE PATH "## STIG: Path of cAttrib lib: libCES.a/CES.lib")
ENDIF()

## Add Module STI
IF(EXISTS ${CMAKE_SOURCE_DIR}/STI)
	CMEXT_ADD_MODULE(STI PATH STI)
ELSE()
	SET(STI_INCLUDE_DIR "" CACHE PATH "## STIG: Root Directory containing headers of STI")
	SET(STI_LIBRARIES   "" CACHE PATH "## STIG: Directory containing libSTI.a/STI.lib")
ENDIF()


## Add Module STIG
CMEXT_ADD_MODULE(STIG)

#CMEXT_SOURCE_GROUP(STI)


