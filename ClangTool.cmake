
SET(LLVM_INCLUDE_DIR	   "" CACHE PATH "Root Directory containing headers of LLVM embedded in cAttrib project")
SET(LLVM_BUILT_INCLUDE_DIR "" CACHE PATH "Root Directory containing built headers of LLVM embedded in cAttrib project")
SET(LLVM_LIBS_DIR          "" CACHE PATH "Directory containing Clang libraries (usualy the same as STIG_CATTRIB_LIB_DIR)")

SET(CLANG_INCLUDE_DIR 	    "" CACHE PATH "Root Directory containing headers of Clang embedded in cAttrib project")
SET(CLANG_BUILT_INCLUDE_DIR "" CACHE PATH "Root Directory containing built headers of Clang embedded in cAttrib project")
SET(CLANG_LIBS_DIR		    "" CACHE PATH "Directory containing Clang libraries (usualy the same as STIG_CATTRIB_LIB_DIR)")

SET(Clang_LIBS
	${CLANG_LIBS_DIR}/libclangFrontend.a 
	${CLANG_LIBS_DIR}/libclangSerialization.a
	${CLANG_LIBS_DIR}/libclangDriver.a
    ${CLANG_LIBS_DIR}/libclangTooling.a
    ${CLANG_LIBS_DIR}/libclangParse.a
    ${CLANG_LIBS_DIR}/libclangSema.a
    ${CLANG_LIBS_DIR}/libclangStaticAnalyzerFrontend.a
    ${CLANG_LIBS_DIR}/libclangStaticAnalyzerCheckers.a
    ${CLANG_LIBS_DIR}/libclangStaticAnalyzerCore.a
    ${CLANG_LIBS_DIR}/libclangAnalysis.a
    ${CLANG_LIBS_DIR}/libclangRewriteFrontend.a
    ${CLANG_LIBS_DIR}/libclangRewrite.a
    ${CLANG_LIBS_DIR}/libclangEdit.a
    ${CLANG_LIBS_DIR}/libclangAST.a
    ${CLANG_LIBS_DIR}/libclangASTMatchers.a
    ${CLANG_LIBS_DIR}/libclangDynamicASTMatchers.a
    ${CLANG_LIBS_DIR}/libclangLex.a
    ${CLANG_LIBS_DIR}/libclangBasic.a
    )

SET(LLVM_LIBS
	${LLVM_LIBS_DIR}/libLLVMAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMBitReader.a
	${LLVM_LIBS_DIR}/libLLVMSupport.a
	${LLVM_LIBS_DIR}/libLLVMMC.a
	${LLVM_LIBS_DIR}/libLLVMMCDisassembler.a
	${LLVM_LIBS_DIR}/libLLVMMCParser.a
	${LLVM_LIBS_DIR}/libLLVMOption.a
	${LLVM_LIBS_DIR}/libLLVMAArch64AsmParser.a
	${LLVM_LIBS_DIR}/libLLVMARMAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMMipsAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMPowerPCAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMSparcAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMSystemZAsmParser.a
	${LLVM_LIBS_DIR}/libLLVMX86AsmParser.a
	)

SET(Clang_DEPENCIES "")

## Clang requires zlib
FIND_PACKAGE( ZLIB REQUIRED )
IF ( ZLIB_FOUND )
    INCLUDE_DIRECTORIES( ${ZLIB_INCLUDE_DIRS} )
    SET(Clang_DEPENCIES ${Clang_DEPENCIES} ${ZLIB_LIBRARIES})
ENDIF( ZLIB_FOUND )

## LLVM requires terminfo
IF(APPLE)
	SET(Clang_DEPENCIES ${Clang_DEPENCIES} -ltermcap)
ENDIF(APPLE)


SET(Clang_INCLUDE_DIRS 
	${CLANG_INCLUDE_DIR}
  	${CLANG_BUILT_INCLUDE_DIR}
  	${LLVM_INCLUDE_DIR}
  	${LLVM_BUILT_INCLUDE_DIR}
  	)

SET(Clang_LIBRARIES ${Clang_LIBS} ${LLVM_LIBS} ${Clang_DEPENCIES})
