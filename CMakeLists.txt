CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

PROJECT(STIG)


###
## Configure Files
###

set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")

SET(SG_ROOT_DIR ${CMAKE_SOURCE_DIR})

SET(SG_LIB_DIR ${CMAKE_SOURCE_DIR}/builds/lib)
SET(SG_INC_DIR ${CMAKE_SOURCE_DIR}/stig/include/)

SET(SG_UTT_DIR ${CMAKE_SOURCE_DIR}/unit_test/)
SET(SG_UTT_RESULTS_DIR ${CMAKE_SOURCE_DIR}/unit_test/results/)

SET(SG_DEVT_DIR ${CMAKE_SOURCE_DIR}/devt/)

SET(SG_CATTRIB_INCLUDE_DIR "" CACHE PATH "## STIG: Root Directory containing headers of cAttrib")
SET(SG_CATTRIB_LIB_DIR	   "" CACHE PATH "## STIG: Directory containing libcAttrib.a")

SET(SG_LLVM_INCLUDE_DIR	   "" CACHE PATH "## STIG: Root Directory containing headers of LLVM embedded in cAttrib project")
SET(SG_LLVM_BUILT_INCLUDE_DIR	   "" CACHE PATH "## STIG: Root Directory containing built headers of LLVM embedded in cAttrib project")
SET(SG_LLVM_LIBS_DIR		"" CACHE PATH "## STIG: Directory containing Clang libraries (usualy the same as SG_CATTRIB_LIB_DIR)")

SET(SG_CLANG_INCLUDE_DIR 	"" CACHE PATH "## STIG: Root Directory containing headers of Clang embedded in cAttrib project")
SET(SG_CLANG_LIBS_DIR		"" CACHE PATH "## STIG: Directory containing Clang libraries (usualy the same as SG_CATTRIB_LIB_DIR)")

SET(SG_USED_CLANG_LIBS
	${SG_CLANG_LIBS_DIR}/libclangFrontend.a 
	${SG_CLANG_LIBS_DIR}/libclangSerialization.a
	${SG_CLANG_LIBS_DIR}/libclangDriver.a
    ${SG_CLANG_LIBS_DIR}/libclangTooling.a
    ${SG_CLANG_LIBS_DIR}/libclangParse.a
    ${SG_CLANG_LIBS_DIR}/libclangSema.a
    ${SG_CLANG_LIBS_DIR}/libclangStaticAnalyzerFrontend.a
    ${SG_CLANG_LIBS_DIR}/libclangStaticAnalyzerCheckers.a
    ${SG_CLANG_LIBS_DIR}/libclangStaticAnalyzerCore.a
    ${SG_CLANG_LIBS_DIR}/libclangAnalysis.a
    ${SG_CLANG_LIBS_DIR}/libclangRewriteFrontend.a
    ${SG_CLANG_LIBS_DIR}/libclangRewrite.a
    ${SG_CLANG_LIBS_DIR}/libclangEdit.a
    ${SG_CLANG_LIBS_DIR}/libclangAST.a
    ${SG_CLANG_LIBS_DIR}/libclangLex.a
    ${SG_CLANG_LIBS_DIR}/libclangBasic.a
    )

SET(SG_USED_LLVM_LIBS
	${SG_LLVM_LIBS_DIR}/libLLVMAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMBitReader.a
	${SG_LLVM_LIBS_DIR}/libLLVMSupport.a
	${SG_LLVM_LIBS_DIR}/libLLVMMC.a
	${SG_LLVM_LIBS_DIR}/libLLVMMCDisassembler.a
	${SG_LLVM_LIBS_DIR}/libLLVMMCParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMOption.a
	${SG_LLVM_LIBS_DIR}/libLLVMAArch64AsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMARMAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMMipsAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMPowerPCAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMSparcAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMSystemZAsmParser.a
	${SG_LLVM_LIBS_DIR}/libLLVMX86AsmParser.a
	)

SET(SG_DEPENCIES_LIBS "")

CONFIGURE_FILE(cmake_config/dev_paths.hpp ${CMAKE_SOURCE_DIR}/devt/source/)
CONFIGURE_FILE(cmake_config/unit_paths.hpp ${CMAKE_SOURCE_DIR}/unit_test/source/)

##############################
## Libraries

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_config")

FIND_PACKAGE( Boost REQUIRED )
FIND_PACKAGE( CppUnit REQUIRED )
#FIND_PACKAGE( TinyXML REQUIRED )

FIND_PACKAGE( ZLIB REQUIRED )
IF ( ZLIB_FOUND )
    INCLUDE_DIRECTORIES( ${ZLIB_INCLUDE_DIRS} )
    SET(SG_DEPENCIES_LIBS ${SG_DEPENCIES_LIBS} ${ZLIB_LIBRARIES})
ENDIF( ZLIB_FOUND )

## LLVM needs terminfo
IF(APPLE)
	SET(SG_DEPENCIES_LIBS ${SG_DEPENCIES_LIBS} -ltermcap)
ENDIF(APPLE)

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")
ENDIF(MSVC)

##############################

###
## Files
###

## Master includes
FILE(	
	GLOB_RECURSE 
	sg_include 
	stig/include/*
	)

## Master sources
FILE(
	GLOB_RECURSE 
	sg_src 
	stig/src/*
	)

## Assets
FILE(
	GLOB_RECURSE 
	sg_assets 
	stig/assets/*
	)
	

###
## Target
###

ADD_EXECUTABLE(
	Stig
	${sg_include}
	${sg_src}
	)


###
## Properties
###

## Set includes dir (master header and module headers)
SET_PROPERTY(
	TARGET Stig
	PROPERTY INCLUDE_DIRECTORIES
  	${CMAKE_SOURCE_DIR}/stig/include/
  	${Boost_INCLUDE_DIR}
  	${SG_CATTRIB_INCLUDE_DIR}
  	${SG_CLANG_INCLUDE_DIR}
  	${SG_LLVM_INCLUDE_DIR}
  	${SG_LLVM_BUILT_INCLUDE_DIR}
)


## Set Target Library
TARGET_LINK_LIBRARIES(
	Stig
	${Boost_LIBRARIES}
	${SG_USED_CLANG_LIBS}
	${SG_USED_LLVM_LIBS}
	${SG_CATTRIB_LIB_DIR}/libcAttrib.a
	${SG_DEPENCIES_LIBS}
	)


## Set output dir (builds)
SET_PROPERTY( TARGET Stig PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/builds/bin)
SET_PROPERTY( TARGET Stig PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/builds/bin)
SET_PROPERTY( TARGET Stig PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/builds/bin)
## Set output dir 


###
## Unit Test
###

FILE(
	GLOB_RECURSE
	unit_sources
	unit_test/source/*
	)

## Master Tests

ADD_EXECUTABLE(
	UnitTest 
	${unit_sources} 
	${sg_include} 
	${sg_src}
	)


## Set output dir 
SET_PROPERTY(TARGET UnitTest PROPERTY RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/unit_test/builds/ )
SET_PROPERTY(TARGET UnitTest PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/unit_test/builds/ )
SET_PROPERTY(TARGET UnitTest PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/unit_test/builds/ )

	
## Set includes dir (master header and module headers)
SET_PROPERTY(
	TARGET UnitTest 
	PROPERTY INCLUDE_DIRECTORIES
  	${CMAKE_SOURCE_DIR}/stig/include/
  	${CMAKE_SOURCE_DIR}/unit_test/include/
  	${Boost_INCLUDE_DIR}
  	${CPPUNIT_INCLUDE_DIRS}
  	${SG_CATTRIB_INCLUDE_DIR}
  	${SG_CLANG_INCLUDE_DIR}
  	${SG_LLVM_INCLUDE_DIR}
  	${SG_LLVM_BUILT_INCLUDE_DIR}
)


## Set Target Library
TARGET_LINK_LIBRARIES(
	UnitTest
	${Boost_LIBRARIES}
	${CPPUNIT_LIBRARIES}
	${SG_USED_CLANG_LIBS}
	${SG_USED_LLVM_LIBS}
	${SG_CATTRIB_LIB_DIR}/libcAttrib.a
	${SG_DEPENCIES_LIBS}
	)
	


###
## Dev Test
###

FILE(
	GLOB_RECURSE
	devt_sources
	devt/source/*
	)

## Master Tests

ADD_EXECUTABLE(
	DevTest 
	${devt_sources} 
	)


## Set output dir 
SET_PROPERTY(TARGET DevTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devt/builds/ )
SET_PROPERTY(TARGET DevTest PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/devt/builds/ )
SET_PROPERTY(TARGET DevTest PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/devt/builds/ )

	
## Set includes dir (master header and module headers)
SET_PROPERTY(
	TARGET DevTest 
	PROPERTY INCLUDE_DIRECTORIES
	${CMAKE_SOURCE_DIR}/stig/include/
  	${CMAKE_SOURCE_DIR}/devt/include/
  	${Boost_INCLUDE_DIR}
  	${CPPUNIT_INCLUDE_DIRS}
  	${SG_CATTRIB_INCLUDE_DIR}
  	${SG_CLANG_INCLUDE_DIR}
  	${SG_LLVM_INCLUDE_DIR}
  	${SG_LLVM_BUILT_INCLUDE_DIR}
)


## Set Target Library
TARGET_LINK_LIBRARIES(
	DevTest
	${Boost_LIBRARIES}
	${CppUnit_LIBRARIES}
	${SG_USED_CLANG_LIBS}
	${SG_USED_LLVM_LIBS}
	${SG_CATTRIB_LIB_DIR}/libcAttrib.a
	${SG_DEPENCIES_LIBS}
	)

####################

###
## Organize
###

SOURCE_GROUP(
	Dev_Tests
	FILES ${devt_sources}
	)

SOURCE_GROUP(
	Unit_Tests
	FILES ${unit_sources}
	)



SOURCE_GROUP(
	Headers
	FILES ${sg_include}
	)
	
SOURCE_GROUP(
	Sources
	FILES ${sg_src}
	)
	
	

INCLUDE(cmake_config/src_group.cmake)
