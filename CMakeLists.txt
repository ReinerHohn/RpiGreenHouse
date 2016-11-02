project(rpiGreenHouse)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/config/cmake/")

include (external_toolgen)
include (macros)

set(REMOTE_EXE_PATH "/home/pi")
set(ENV_VARS LD_LIBRARY_PATH=${REMOTE_EXE_PATH})

file(WRITE "${CMAKE_SOURCE_DIR}/QtCreatorDeployment.txt" "${REMOTE_EXE_PATH}\n")
add_deployment_file(thirdparty/libprotobuf-c.so.1 .)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -gdwarf-3") #std=gnu99
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -Wpedantic") #" -Wdocumentation -Weverything")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

set(LIBRARIES "")

add_subdirectory(library/socket)
add_subdirectory(thirdparty/CUnit)
add_subdirectory(app)

message(SRCS ist ${_SRCS})
option(BUILD_UTEST "Build Unittests" OFF)

if(NOT BUILD_UTEST)
#    create_doxygen()
endif()


