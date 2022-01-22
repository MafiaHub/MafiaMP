include(UpdateGitHash)

update_git_version(MAFIAMP "${CMAKE_CURRENT_SOURCE_DIR}/shared/version.cpp.in" "${CMAKE_BINARY_DIR}/mafiamp_version.cpp")

include_directories(${CMAKE_SOURCE_DIR}/code/framework ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    add_subdirectory(client)
    add_subdirectory(launcher)
    add_subdirectory(worker)
endif()

add_subdirectory(server)
