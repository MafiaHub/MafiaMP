set(MAFIAMP_SERVER_FILES
    src/main.cpp
    src/core/server.cpp

    src/core/builtins/human.cpp
    src/core/builtins/player.cpp
    src/core/builtins/vehicle.cpp

    ../shared/entities/register_entities.cpp

    ${CMAKE_BINARY_DIR}/mafiamp_version.cpp
)

add_executable(MafiaMPServer ${MAFIAMP_SERVER_FILES})
target_include_directories(MafiaMPServer PRIVATE src)
target_link_libraries(MafiaMPServer Framework::Server)
# libnode and the crash handler next to the binary, $ORIGIN on Linux
fw_stage_runtime(MafiaMPServer SERVER)

# Match FrameworkServer's compile definitions
if(WIN32)
    target_compile_definitions(MafiaMPServer PRIVATE NOMINMAX _USE_MATH_DEFINES)
endif()
