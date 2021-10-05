set(MAFIAMP_LAUNCHER_FILES
    src/main.cpp
)

add_executable(MafiaMPLauncher ${MAFIAMP_LAUNCHER_FILES})
target_include_directories(MafiaMPLauncher PRIVATE .)
target_link_libraries(MafiaMPLauncher PRIVATE Framework FrameworkLoader MafiaMPClient ntdll)

target_link_options(MafiaMPLauncher PRIVATE /IGNORE:4254 /SAFESEH:NO /BASE:0x1400000000 /INCREMENTAL:NO /SUBSYSTEM:windows /DYNAMICBASE:NO /LARGEADDRESSAWARE /MERGE:.text=.zdata /LAST:.zdata)

target_link_directories(MafiaMPLauncher PRIVATE ../../vendors/ntdll)
target_link_libraries(MafiaMPLauncher PRIVATE ntdll_x64)
