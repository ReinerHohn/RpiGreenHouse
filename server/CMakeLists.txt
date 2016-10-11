project(rpiGreenHouse_server)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)


#file(WRITE "${CMAKE_SOURCE_DIR}/QtCreatorDeployment.txt" "/home/pi\n")

macro(add_deployment_file SRC DEST)
    file(RELATIVE_PATH path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    file(APPEND "${CMAKE_SOURCE_DIR}/QtCreatorDeployment.txt" "${path}/${SRC}:${DEST}\n")
endmacro()

macro(add_deployment_directory SRC DEST)
    file(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SRC}/*")
    foreach(filename ${files})
        get_filename_component(path ${filename} PATH)
        add_deployment_file("${filename}" "${DEST}/${path}")
    endforeach(filename)
endmacro()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -gdwarf-3") #std=gnu99

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

list(APPEND SRC_LIST_SERVER    	main_server.c
                                spiComm.h       spiComm.c
                                socketComm.h    socketComm.c
                                cc3200CapSens.c cc3200CapSens.h #mcp3008.c     mcp3008.h
                                gpio.c          gpio.h
                                pump.c          pump.h
                                unittestc.c unittestc.h)

#add_library(SOCKETLIB SHARED socketComm.h    socketComm.c)

#add_deployment_file(${PROJECT_NAME}  "/home/pi")

add_executable("${PROJECT_NAME}" ${SRC_LIST_SERVER})

#target_link_libraries(${PROJECT_NAME} SOCKETLIB)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/CUnit/include)
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/CUnit/lib/libcunit.a)
