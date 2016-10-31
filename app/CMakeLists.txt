option(BUILD_SERVER "Build Server App" OFF)
option(BUILD_CLIENT "Build Client App" OFF)

if(BUILD_SERVER)
	add_subdirectory(server)
endif()

if(BUILD_CLIENT)
	add_subdirectory(client)
endif()
