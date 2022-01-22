set(MAFIAMP_WORKER_FILES
    src/main.cpp
)

add_executable(MafiaMPWorker ${MAFIAMP_WORKER_FILES})
target_include_directories(MafiaMPWorker PRIVATE .)
target_link_libraries(MafiaMPWorker PRIVATE Framework FrameworkClient)
