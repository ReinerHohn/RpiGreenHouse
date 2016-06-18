project(rpiGreenHouse)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)

list(APPEND SRC_LIST main.cpp mcp3008Spi.cpp mcp3008Spi.h)
add_executable(${PROJECT_NAME} ${SRC_LIST})

