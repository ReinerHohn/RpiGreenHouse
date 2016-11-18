set(module_name cunit)

if(BUILD_UTEST)
	list(APPEND SRC_CUNIT Sources/Automated/Automated.c)
	list(APPEND SRC_CUNIT Sources/Framework/CUError.c)
	list(APPEND SRC_CUNIT Sources/Framework/MyMem.c)
	list(APPEND SRC_CUNIT Sources/Framework/TestDB.c)
	list(APPEND SRC_CUNIT Sources/Framework/TestRun.c)
	list(APPEND SRC_CUNIT Sources/Framework/Util.c)

	add_library(${module_name} ${SRC_CUNIT})
	target_include_directories(${module_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Headers)
	
	list(APPEND libs_temp ${module_name})
	set(LIBRARIES ${LIBRARIES} ${libs_temp} PARENT_SCOPE)

        set(_SRCS ${_SRCS} ${SRC_CUNIT} PARENT_SCOPE)
endif()
