set(module_name serial)

if(BUILD_SERIAL)
	list(APPEND SRC_SERIAL    message.pb-c.c    message.pb-c.h)
endif()

list(APPEND modules "libprotobuf-c")
load_pkg_libs(${modules})

link_directories(           ${LIB_DIRS})
add_library(                ${module_name} ${SRC_SERIAL} ${SRC_SERIAL_UTEST} )
target_include_directories( ${module_name} PRIVATE  ${INC_DIRS} )
target_include_directories( ${module_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)
target_link_libraries(      ${module_name} ${LIBS})
target_compile_options(     ${module_name} PRIVATE ${CFLAGS_OTHER})

# Add list of libraries to global list
list(APPEND libs_temp ${module_name})
set(LIBRARIES ${LIBRARIES} ${libs_temp} PARENT_SCOPE)

set(_SRCS ${SRC_SERIAL} ${SRC_SERIAL_UTEST} PARENT_SCOPE)

