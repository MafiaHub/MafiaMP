include_directories(${CMAKE_SOURCE_DIR}/code/framework)

if(WIN32)
    add_subdirectory(client)
    add_subdirectory(launcher)
endif()

add_subdirectory(server)
