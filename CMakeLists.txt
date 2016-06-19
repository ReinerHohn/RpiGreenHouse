project(rpiGreenHouse)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")

list(APPEND SRC_LIST    main.c
                        spiComm.h spiComm.c
                        socketComm.h socketComm.c
                        mcp3008.c mcp3008.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})

