set(module_name socket)

if(BUILD_SOCK_SERVER)
	list(APPEND SRC_SOCKET    	socketServer.h    socketServer.c)
endif()

if(BUILD_SOCK_CLIENT)
	list(APPEND SRC_SOCKET    	socketClient.h    socketClient.c)
endif()

if(BUILD_UTEST)
	    list(APPEND SRC_SOCKET_UTEST utest/socketTest.c)
	    list(APPEND SRC_SOCKET_UTEST utest/socketTest.h)
endif()

add_library(${module_name} ${SRC_SOCKET} ${SRC_SOCKET_UTEST} )
target_include_directories(${module_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)

list(APPEND libs_temp ${module_name})
set(LIBRARIES ${LIBRARIES} ${libs_temp} PARENT_SCOPE)
set(_SRCS ${_SRCS} ${SRC_SOCKET} ${SRC_SOCKET_UTEST} PARENT_SCOPE)


