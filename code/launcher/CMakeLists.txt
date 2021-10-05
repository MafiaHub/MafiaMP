set(MAFIAMP_LAUNCHER_FILES
    src/tls.cpp
    src/main.cpp
)

add_executable(MafiaMPLauncher ${MAFIAMP_LAUNCHER_FILES})
target_include_directories(MafiaMPLauncher PRIVATE .)
target_link_libraries(MafiaMPLauncher PRIVATE Framework FrameworkLoader MafiaMPClient)

target_compile_options(MafiaMPLauncher PRIVATE /UNICODE)
target_link_options(MafiaMPLauncher PRIVATE /IGNORE:4254 /SAFESEH:NO /DYNAMICBASE:NO /BASE:0x140000000 /LARGEADDRESSAWARE /SUBSYSTEM:windows /SECTIONLAYOUT:@${CMAKE_CURRENT_SOURCE_DIR}/SectionLayout.txt)

target_link_directories(MafiaMPLauncher PRIVATE ../../vendors/ntdll)
target_link_libraries(MafiaMPLauncher PRIVATE ntdll_x64)
