set(MAFIAMP_SERVER_FILES
    src/main.cpp
)

add_executable(MafiaMPServer ${MAFIAMP_SERVER_FILES})
target_include_directories(MafiaMPServer PRIVATE .)
target_link_libraries(MafiaMPServer Framework FrameworkServer)