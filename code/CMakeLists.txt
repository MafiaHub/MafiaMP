include(UpdateGitHashMafiaMP)

update_git_version_mafiamp()

include_directories(${CMAKE_SOURCE_DIR}/code/framework ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    add_subdirectory(client)
    add_subdirectory(launcher)
endif()

add_subdirectory(server)
