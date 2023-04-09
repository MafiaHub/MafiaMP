include(UpdateGitHash)

update_git_version(MAFIAMP "${CMAKE_CURRENT_SOURCE_DIR}/shared/version.cpp.in" "${CMAKE_BINARY_DIR}/mafiamp_version.cpp")

include_directories(${CMAKE_SOURCE_DIR}/code/framework ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CL_64)
    add_subdirectory(client)
    add_subdirectory(client-worker)
    add_subdirectory(launcher)
endif()

add_subdirectory(server)
