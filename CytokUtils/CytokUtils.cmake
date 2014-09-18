

CMEXT_ADD_FILE_DIR(CytokUtils ckutils include)
CMEXT_ADD_FILE_DIR(CytokUtils ckutils lib)

SET(ckutils_LINK_LIBS 
	)

CMEXT_ADD_LIBRARY(CytokUtils ckutils STATIC)


#CMEXT_ADD_UNIT_TEST(STIG stig-unit)

SET(CytokUtils_INCLUDE_DIR ${CytokUtils_ROOT_DIR_GLOBAL}/include)
SET(CytokUtils_LIBRARIES   ${LIB_DIR}/${ckutils_STATIC_LIB_NAME} )

cmext_source_group(CytokUtils include)