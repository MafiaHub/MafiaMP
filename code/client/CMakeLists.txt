set(MAFIAMP_CLIENT_FILES
    src/main.cpp
)

set(MAFIAMP_SDK_FILES
    src/sdk/ue/sys/core/c_scene_object.cpp
    src/sdk/ue/sys/core/i_core.cpp
    src/sdk/ue/sys/math/c_matrix.cpp
    src/sdk/ue/sys/math/c_quat.cpp
    src/sdk/ue/sys/math/c_vector.cpp
    src/sdk/c_game_input_module.cpp
    src/sdk/c_game.cpp
    src/sdk/c_ie.cpp
    src/sdk/c_ticked_module_manager.cpp
    src/sdk/c_time_budget_info.cpp
    src/sdk/patterns.cpp
)

add_library(MafiaMPClient SHARED ${MAFIAMP_CLIENT_FILES} ${MAFIAMP_SDK_FILES})
target_include_directories(MafiaMPClient PRIVATE .)
target_link_libraries(MafiaMPClient Framework FrameworkClient)
target_link_options(MafiaMPClient PRIVATE /SAFESEH:NO /MANIFEST:NO)
