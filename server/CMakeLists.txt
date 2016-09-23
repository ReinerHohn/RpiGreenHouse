project(rpiGreenHouse_server)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -gdwarf-3") #std=gnu99

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

list(APPEND SRC_LIST_SERVER    	main_server.c
                                spiComm.h       spiComm.c
                                socketComm.h    socketComm.c
                                cc3200CapSens.c cc3200CapSens.h #mcp3008.c     mcp3008.h
                                gpio.c          gpio.h
                                pump.c          pump.h)

add_executable("${PROJECT_NAME}" ${SRC_LIST_SERVER})
