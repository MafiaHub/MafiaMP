set(MAFIAMP_SERVER_FILES
    src/main.cpp
    src/core/server.cpp

    src/core/builtins/human.cpp
    src/core/builtins/player.cpp
    src/core/builtins/vehicle.cpp

    src/core/modules/environment.cpp
    src/core/modules/human.cpp
    src/core/modules/vehicle.cpp

    ${CMAKE_BINARY_DIR}/mafiamp_version.cpp
)

add_executable(MafiaMPServer ${MAFIAMP_SERVER_FILES})
target_include_directories(MafiaMPServer PRIVATE src)
target_link_libraries(MafiaMPServer Framework FrameworkServer)
