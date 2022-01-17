set(MAFIAMP_LAUNCHER_FILES
    src/main.cpp
    ${CMAKE_BINARY_DIR}/mafiamp_version.cpp
)

add_executable(MafiaMPLauncher ${MAFIAMP_LAUNCHER_FILES})
target_include_directories(MafiaMPLauncher PRIVATE .)
target_link_libraries(MafiaMPLauncher PRIVATE Framework FrameworkLoader MafiaMPClient)
