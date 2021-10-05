set(MAFIAMP_CLIENT_FILES
    src/main.cpp
)

add_library(MafiaMPClient SHARED ${MAFIAMP_CLIENT_FILES})
target_include_directories(MafiaMPClient PRIVATE .)
target_link_libraries(MafiaMPClient Framework FrameworkClient)
target_link_options(MafiaMPClient PRIVATE /SAFESEH:NO)
