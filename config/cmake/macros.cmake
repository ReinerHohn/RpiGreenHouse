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

macro(load_pkg_libs modules)
    # Pkg-config
    find_package(PkgConfig)
    set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/lib/arm-linux-gnueabihf/pkgconfig/")

    foreach(module ${modules})
        pkg_search_module(PREF REQUIRED ${module})
        list(APPEND LIBS2 ${PREF_LIBRARIES})
        list(APPEND INC_DIRS2 ${PREF_INCLUDE_DIRS})
        list(APPEND LIB_DIRS2 ${PREF_LIBRARY_DIRS})
        list(APPEND CFLAGS_OTHER2 ${PREF_CFLAGS_OTHER})

        set(LIBS            ${LIBS}         ${LIBS2}            PARENT_SCOPE)
        set(INC_DIRS        ${INC_DIRS}     ${INC_DIRS2}        PARENT_SCOPE)
        set(LIB_DIRS        ${LIB_DIRS}     ${LIB_DIRS2}        PARENT_SCOPE)
        set(CFLAGS_OTHER    ${CFLAGS_OTHER} ${CFLAGS_OTHER2}    PARENT_SCOPE)

    endforeach()
endmacro()
