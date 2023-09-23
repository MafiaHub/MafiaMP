set(MAFIAMP_CLIENT_WORKER_FILES
    src/main.cpp
    src/application.cpp
)

add_executable(MafiaMPClientWorker ${MAFIAMP_CLIENT_WORKER_FILES})
target_include_directories(MafiaMPClientWorker PRIVATE src)
target_link_libraries(MafiaMPClientWorker Framework CEF)
target_link_options(MafiaMPClientWorker PRIVATE /SAFESEH:NO /MANIFEST:NO /ENTRY:WinMainCRTStartup /SUBSYSTEM:WINDOWS)
