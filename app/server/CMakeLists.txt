
list(APPEND SRC_SERVER    	main_server.c)
set(APP_NAME ${PROJECT_NAME}_server)


list(APPEND SRC_SERVER    	spiComm.h       spiComm.c)
list(APPEND SRC_SERVER    	cc3200CapSens.c cc3200CapSens.h)
list(APPEND SRC_SERVER    	mcp3008.c       mcp3008.h)
list(APPEND SRC_SERVER    	gpio.c          gpio.h)
list(APPEND SRC_SERVER    	pump.c          pump.h)

list(APPEND SRC_SERVER    	${CMAKE_CURRENT_SOURCE_DIR}/../common/message.pb-c.c ${CMAKE_CURRENT_SOURCE_DIR}/../common/message.pb-c.h)
INCLUDE_DIRECTORIES(../common)


list(APPEND modules "libprotobuf-c")

load_pkg_libs(modules)
link_directories(${LIB_DIRS})
#link_directories(${PROTOBUF_LIBRARY_DIRS})
add_executable(${APP_NAME}      ${SRC_SERVER})

target_link_libraries(${APP_NAME} ${LIBRARIES})
target_include_directories(${APP_NAME} PUBLIC ${INC_DIRS})
target_compile_options(${APP_NAME} PUBLIC ${CFLAGS_OTHER})

#target_link_libraries(${APP_NAME} ${LIBRARIES} ${PROTOBUF_LIBRARIES})
#target_include_directories(${APP_NAME} PUBLIC ${PROTOBUF_INCLUDE_DIRS})
#target_compile_options(${APP_NAME} PUBLIC ${PROTOBUF_CFLAGS_OTHER})

# Qt creator stuff
set(PROJ_TARGET rpi_server)
set(TARGET_UUID "b91582c8-2694-46d2-8264-c37861ff70b2") # SERVER

# Generate CMakeLists.txt.user (holding project information)
create_qtcreator()

# Sources for qtcreator
list(APPEND _SRCS ${SRC_SERVER} PARENT_SCOPE)
