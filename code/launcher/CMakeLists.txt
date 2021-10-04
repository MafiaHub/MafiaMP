set(MAFIAMP_LAUNCHER_FILES
    src/main.cpp
    src/win32_cfg.cpp
)

add_executable(MafiaMPLauncher ${MAFIAMP_LAUNCHER_FILES})
target_include_directories(MafiaMPLauncher PRIVATE .)
target_link_libraries(MafiaMPLauncher PRIVATE Framework FrameworkClient MafiaMPClient)
target_link_options(MafiaMPLauncher PRIVATE /IGNORE:4254 /SAFESEH:NO /DYNAMICBASE:NO /SUBSYSTEM:Windows /LARGEADDRESSAWARE /LAST:.zdata)
