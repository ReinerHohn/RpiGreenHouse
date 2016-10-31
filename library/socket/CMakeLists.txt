set(module_name socket)

option(BUILD_SOCK_SERVER "Build Socket Server" OFF)
option(BUILD_SOCK_CLIENT "Build Socket Client" OFF)

include_directories(${CMAKE_CURRENT_LIST_DIR})


if(BUILD_SOCK_SERVER)
	list(APPEND SRC_SOCKET    	socketServer.h    socketServer.c)
	if(BUID_UTEST)
	    list(APPEND SRC_SOCKET_UTEST utest/socketServerTest.c)
	    list(APPEND SRC_SOCKET_UTEST utest/socketServerTest.h)
	endif()
endif()

if(BUILD_SOCK_CLIENT)
	list(APPEND SRC_SOCKET    	socketClient.h    socketClient.c)
	if(BUID_UTEST)
	    list(APPEND SRC_SOCKET_UTEST utest/socketClientTest.c)
	    list(APPEND SRC_SOCKET_UTEST utest/socketClientTest.h)
	endif()
endif()

add_library(${module_name} ${SRC_SOCKET} ${SRC_SOCKET_UTEST} )
target_include_directories(${module_name} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/)

set(LIBRARIES "${module_name}" PARENT_SCOPE)

list(APPEND _SRCS ${SRC_SOCKET} ${SRC_SOCKET_UTEST} PARENT_SCOPE)


