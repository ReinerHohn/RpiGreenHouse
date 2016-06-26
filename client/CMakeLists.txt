project(rpiGreenHouse_client)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

list(APPEND SRC_LIST_CLIENT 	"main_client.c"
				"socketClient.c"
				"socketClient.h")
add_executable("${PROJECT_NAME}" ${SRC_LIST_CLIENT})

