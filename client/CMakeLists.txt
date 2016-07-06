project(rpiGreenHouse_client C)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu99")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

list(APPEND SRC_LIST_CLIENT 	"main_client.c"
                                "socketClient.c"    "socketClient.h"
                                "fifo.c"            "fifo.h"
                                "mysqlWrapper.c"    "mysqlWrapper.h")

add_executable("${PROJECT_NAME}" ${SRC_LIST_CLIENT})

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_FIND_ROOT_PATH}/usr/include/mysql/")
target_link_libraries("${PROJECT_NAME}" PRIVATE "${CMAKE_FIND_ROOT_PATH}/usr/lib/arm-linux-gnueabihf/libmysqlclient.so")


message("${CMAKE_FIND_ROOT_PATH}/usr/include/mysql/")
