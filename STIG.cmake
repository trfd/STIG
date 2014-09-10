
CMEXT_REQUIRE(Clang_LIBRARIES   "STI requires Clang_LIBRARIES")
CMEXT_REQUIRE(STI_LIBRARIES     "STI requires STI_LIBRARIES")
CMEXT_REQUIRE(CAttrib_LIBRARIES "STI requires CAttrib_LIBRARIES")

CMEXT_ADD_FILE_DIR(STIG stig/include)
CMEXT_ADD_FILE_DIR(STIG stig/src)

SET(STIG_LINK_LIBS 
	${Clang_LIBRARIES}
	${CAttrib_LIBRARIES}
	${STI_LIBRARIES}
	)

CMEXT_ADD_EXECUTABLE(STIG)

#CMEXT_ADD_UNIT_TEST(STIG stig-unit)

SET(STIG_INCLUDE_DIR ${STIG_ROOT_DIR_GLOBAL}/stig/include)
SET(STIG_LIBRARIES   ${LIB_DIR}/${STIG_STATIC_LIB_NAME})
