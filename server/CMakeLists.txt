project(rpiGreenHouse_server)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../config/cmake/")

include (external_toolgen)
include (macros)

INCLUDE_DIRECTORIES(.)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -gdwarf-3") #std=gnu99
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -Wpedantic -Wdocumentation -Weverything")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

list(APPEND SRC_SERVER    	main_server.c)
list(APPEND SRC_SERVER    	spiComm.h       spiComm.c)

list(APPEND SRC_SERVER    	socket/socketComm.h    socket/socketComm.c)
if(BUID_UTEST)
    list(APPEND SRC_SERVER_UTEST socket/utest/socketTest.c)
    list(APPEND SRC_SERVER_UTEST socket/utest/socketTest.h)
endif()

list(APPEND SRC_SERVER    	cc3200CapSens.c cc3200CapSens.h)
list(APPEND SRC_SERVER    	mcp3008.c       mcp3008.h)
list(APPEND SRC_SERVER    	gpio.c          gpio.h)
list(APPEND SRC_SERVER    	pump.c          pump.h)

if(BUID_UTEST)
#    list(APPEND SRC_SERVER_UTEST utest/unittestc.c utest/unittestc.h)
    list(APPEND SRC_SERVER_UTEST utest/cutester.c)
endif()

if(NOT BUID_UTEST)
    set(_SRCS ${SRC_SERVER})
    create_doxygen()
endif()


add_executable("${PROJECT_NAME}" ${SRC_SERVER})
if(BUID_UTEST)
    add_executable("${PROJECT_NAME}_utest" ${SRC_SERVER_UTEST})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/CUnit/include)
    target_link_libraries("${PROJECT_NAME}_utest" "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/CUnit/lib/libcunit.a")
endif()

#add_deployment_file(${PROJECT_NAME}  "/home/pi")
#add_library(SOCKETLIB SHARED socketComm.h    socketComm.c)
#target_link_libraries(${PROJECT_NAME} SOCKETLIB)




