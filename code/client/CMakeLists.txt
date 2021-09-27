set(MAFIAMP_CLIENT_FILES
    src/main.cpp
)

add_library(MafiaMPClient ${MAFIAMP_CLIENT_FILES})
target_include_directories(MafiaMPClient PRIVATE .)
target_link_libraries(MafiaMPClient Framework FrameworkClient)