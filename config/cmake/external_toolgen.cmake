macro(init)
	SET(YAML_FILE ${CMAKE_BINARY_DIR}/config.yml)
	SET(DOXY_FILE ${CMAKE_BINARY_DIR}/Doxygen.Doxyfile)
endmacro(init)

# Generate CMakeLists.txt.user (holding project information)
function(create_qtcreator)
	init()	
	create_yaml()
	message(YAML_FILE ${YAML_FILE})
	execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/config/qtcreator_project.py ${YAML_FILE})
endfunction(create_qtcreator)

function(create_doxygen)
	init()	
	create_yaml()
	execute_process(COMMAND ${CMAKE_SOURCE_DIR}/../config/external_toolgen.py ${YAML_FILE} -d)
	execute_process(COMMAND doxygen ${DOXY_FILE})
endfunction(create_doxygen)

function(create_yaml)
	init()

	file(WRITE ${YAML_FILE} "proj_name: ${APP_NAME}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "target_name: ${PROJ_TARGET}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "target_uuid: ${TARGET_UUID}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "source_dir: ${CMAKE_SOURCE_DIR}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "build_dir: ${CMAKE_BINARY_DIR}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "objdir: ${CMAKE_BINARY_DIR}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "build_loc: ${CMAKE_SOURCE_DIR}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "svn_root: ${CMAKE_SOURCE_DIR}")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	#file(APPEND ${YAML_FILE} "workspace_dir: $(WORKSPACE_DIR)")
	#file(APPEND ${YAML_FILE} "ip_target: $(REMOTE)")
	#file(APPEND ${YAML_FILE} "remote_user: $(REMOTE_USER)")
	#file(APPEND ${YAML_FILE} "remote_target_path: $(REMOTE_EXE_PATH)$(TARGET)")
	#file(APPEND ${YAML_FILE} "prerun_cmd: $(PRE_RUN_CMD)")
	#file(APPEND ${YAML_FILE} "prog_args: $(REMOTE_ARGS)")
	#file(APPEND ${YAML_FILE} "gdb_exe: $(CROSS_COMPILE)gdb")
	file(APPEND ${YAML_FILE} "defines:")
	file(APPEND ${YAML_FILE} "\r\n")
	file(APPEND ${YAML_FILE} "   - \"name\": \"REINER\"")
	file(APPEND ${YAML_FILE} "\r\n")
	file(APPEND ${YAML_FILE} "   - \"name\": \"WERNER\"")
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "env_vars:")
	foreach(var ${ENV_VARS})
		file(APPEND ${YAML_FILE} "\r\n")
		file(APPEND ${YAML_FILE} "   - ${var} ")
	endforeach(var)
	file(APPEND ${YAML_FILE} "\r\n\r\n")
#	execute_process( $(CMAKE_C_COMPILER) $(CMAKE_CXX_FLAGS) $(CMAKE_C_FLAGS) -E -dM - | awk '{print "   - name: "$$2; $$1=""; $$2=""; gsub(/^[\t ]+/, "", $$0); gsub(/"/, "\\\"", $$0); print "     value: \""$$0"\""}'  RESULT_VARAIBLE C_DEFINES) 
#	file(APPEND ${YAML_FILE} "${RESULT_VARAIBLE}")
	file(APPEND ${YAML_FILE} "includes:")
	file(APPEND ${YAML_FILE} "\r\n")
	file(APPEND ${YAML_FILE} "   - REINER")
#	file(APPEND ${YAML_FILE}  | LANG=C $(shell $(CMAKE_C_COMPILER) -print-prog-name=cc1) $(CMAKE_CXX_FLAGS) $(CMAKE_C_FLAGS) -v 2>&1 | awk '/End of search list./{exit}; /^ /{gsub(/^ /, "   - ", $$0); print $$0}' )
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "c_files:")

	foreach(F ${_SRCS})
		file(APPEND ${YAML_FILE} "\r\n")
		file(APPEND ${YAML_FILE}  "   - ${F}")
	endforeach()
	
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "sources:")
	foreach(F ${_SRCS})
		get_filename_component(RES ${F} ABSOLUTE)
		file(APPEND ${YAML_FILE} "\r\n")
		file(APPEND ${YAML_FILE}  "   - ${RES}")
	endforeach()
	file(APPEND ${YAML_FILE} "\r\n\r\n")
	file(APPEND ${YAML_FILE} "files:")
	
	#if MISC_FILES
	#	for F in $(shell realpath $(MISC_FILES)); do echo "   - $$F"); done
	#endif
	#file(APPEND ${YAML_FILE} "   - $(PROJECT_ROOT)/Makefile")
endfunction(create_yaml)
