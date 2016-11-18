if(NOT BUILD_UTEST)
    list(APPEND SRC_CLIENT    	main_client.c)
    set(APP_NAME ${PROJECT_NAME}_client)
else()
    list(APPEND SRC_CLIENT      utest/cutester.c)
    set(APP_NAME ${PROJECT_NAME}_client_utest)
endif()

list(APPEND SRC_CLIENT    	fifo.h       fifo.c)
list(APPEND SRC_CLIENT    	mysqlWrapper.c mysqlWrapper.h)
list(APPEND SRC_CLIENT    	dataB.c       dataB.h)


list(APPEND SRC_CLIENT    	${CMAKE_CURRENT_SOURCE_DIR}/../common/message.pb-c.c ${CMAKE_CURRENT_SOURCE_DIR}/../common/message.pb-c.h)

INCLUDE_DIRECTORIES(../common)

link_directories(${PROTOBUF_LIBRARY_DIRS})
add_executable(${APP_NAME}      ${SRC_CLIENT})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu99") # -gdwarf-3
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

target_link_libraries(${APP_NAME} ${LIBRARIES} ${PROTOBUF_LIBRARIES})
#target_link_libraries("${APP_NAME}" PRIVATE "${CMAKE_FIND_ROOT_PATH}/usr/lib/arm-linux-gnueabihf/libmysqlclient.so")
target_include_directories(${APP_NAME} PUBLIC ${PROTOBUF_INCLUDE_DIRS})
target_include_directories("${APP_NAME}" PRIVATE "${CMAKE_FIND_ROOT_PATH}/usr/include/mysql/")
target_compile_options(${APP_NAME} PUBLIC ${PROTOBUF_CFLAGS_OTHER})


# Qt creator stuff
set(PROJ_TARGET rpi_client)
set(TARGET_UUID "e0e941c2-ac8b-48d3-839c-f4c4c7039296") # CLIENT

# Generate CMakeLists.txt.user (holding project information)
create_qtcreator()

# Sources for qtcreator
list(APPEND _SRCS ${SRC_CLIENT} PARENT_SCOPE)
