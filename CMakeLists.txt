project(rpiGreenHouse)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

#list(APPEND SRC_LIST main.cpp mcp3008Spi.cpp mcp3008Spi.h)
list(APPEND SRC_LIST spidev_test.c)
add_executable(${PROJECT_NAME} ${SRC_LIST})

