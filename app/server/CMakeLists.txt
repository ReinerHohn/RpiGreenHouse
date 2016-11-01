if(NOT BUILD_UTEST)
list(APPEND SRC_SERVER    	main_server.c)
endif()

list(APPEND SRC_SERVER    	spiComm.h       spiComm.c)

list(APPEND SRC_SERVER    	cc3200CapSens.c cc3200CapSens.h)
list(APPEND SRC_SERVER    	mcp3008.c       mcp3008.h)
list(APPEND SRC_SERVER    	gpio.c          gpio.h)
list(APPEND SRC_SERVER    	pump.c          pump.h)

set(APP_NAME ${PROJECT_NAME}_server)
set(APP_NAME_UTEST ${APP_NAME}_utest)

message(LIBRARIES ${LIBRARIES})

if(BUILD_UTEST)
    list(APPEND SRC_SERVER_UTEST utest/cutester.c)

    add_executable(${APP_NAME_UTEST}  ${SRC_SERVER} ${SRC_SERVER_UTEST})
    target_link_libraries(${APP_NAME_UTEST} ${LIBRARIES})
endif()

#add_executable("${APP_NAME}" ${SRC_SERVER})
#target_link_libraries(${APP_NAME} ${LIBRARIES})

list(APPEND _SRCS ${SRC_SERVER} PARENT_SCOPE)




