set(MAFIAMP_CLIENT_FILES
    src/main.cpp
    src/core/application.cpp
    src/game/module.cpp
    src/game/hooks/application.cpp
    src/game/hooks/fast_load.cpp
    src/game/hooks/loading_screen.cpp
    src/game/hooks/mod_init.cpp
)

set(MAFIAMP_SDK_FILES
    src/sdk/entities/human/c_human_inventory.cpp
    src/sdk/entities/human/c_human_script.cpp
    src/sdk/entities/player/c_player_model_manager.cpp
    src/sdk/entities/c_car.cpp
    src/sdk/entities/c_door.cpp
    src/sdk/entities/c_entity.cpp
    src/sdk/entities/c_entity_list.cpp
    src/sdk/entities/c_player_2.cpp
    src/sdk/entities/c_vehicle.cpp
    src/sdk/inventory/c_inventory_wrapper.cpp
    src/sdk/mafia/framework/c_mafia_framework.cpp
    src/sdk/mafia/streaming/c_actors_slot_wrapper.cpp
    src/sdk/mafia/streaming/c_slot_wrapper.cpp
    src/sdk/mafia/streaming/c_streaming_module.cpp
    src/sdk/mafia/ui/menu/c_save_menu.cpp
    src/sdk/mafia/ui/navigation/c_navigation.cpp
    src/sdk/mafia/ui/c_game_gui_2_module.cpp
    src/sdk/ue/game/anim/c_wanim_playback_manager.cpp
    src/sdk/ue/game/camera/c_camera.cpp
    src/sdk/ue/game/camera/c_game_camera.cpp
    src/sdk/ue/game/humainai/c_character_controller.cpp
    src/sdk/ue/game/humainai/c_character_state_handler.cpp
    src/sdk/ue/game/human/c_behavior_character.cpp
    src/sdk/ue/gfx/environmenteffects/c_gfx_environment_effects.cpp
    src/sdk/ue/gfx/environmenteffects/c_weather_manager_2.cpp
    src/sdk/ue/sys/core/c_scene_object.cpp
    src/sdk/ue/sys/core/i_core.cpp
    src/sdk/ue/sys/math/c_matrix.cpp
    src/sdk/ue/sys/math/c_quat.cpp
    src/sdk/ue/sys/math/c_vector.cpp
    src/sdk/ue/c_string.cpp
    src/sdk/wrappers/c_human_2_car_wrapper.cpp
    src/sdk/c_game.cpp
    src/sdk/c_game_input_module.cpp
    src/sdk/c_ie.cpp
    src/sdk/c_ticked_module_manager.cpp
    src/sdk/c_time_budget_info.cpp
    src/sdk/patterns.cpp
)

add_library(MafiaMPClient SHARED ${MAFIAMP_CLIENT_FILES} ${MAFIAMP_SDK_FILES})
target_include_directories(MafiaMPClient PRIVATE .)
target_link_libraries(MafiaMPClient Framework FrameworkClient shlwapi)
target_link_options(MafiaMPClient PRIVATE /SAFESEH:NO /MANIFEST:NO)

add_custom_command(TARGET MafiaMPClient POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/../../files" "${CMAKE_BINARY_DIR}/bin/files/"
            )
