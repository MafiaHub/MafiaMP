#pragma once

#include <cstdint>

namespace SDK::ue::game::anim {
    /**
     * Animation behavior variables used with C_BehaviorCharacter::SetWAnimVariable/GetWAnimVariable.
     * These control various aspects of character animation blending and IK.
     */
    enum E_WAnimBehaviorVar : int32_t {
        E_WABV_SPEED                 = 0,  // Parametric movement speed (walk/run blend)
        E_WABV_ANGLE                 = 1,  // Movement angle
        E_WABV_DIRECTION             = 2,  // Movement direction
        E_WABV_DISTANCE              = 3,  // Distance parameter
        E_WABV_HEIGHT                = 4,  // Height parameter
        E_WABV_WIDTH                 = 5,  // Width parameter
        E_WABV_SLOPE                 = 6,  // Ground slope angle
        E_WABV_COVER_HEIGHT          = 7,  // Cover height
        E_WABV_DEST_COVER_HEIGHT     = 8,  // Destination cover height
        E_WABV_BLINDFIRE_HEIGHT      = 9,  // Blind fire height
        E_WABV_PITCH                 = 10, // Head/aim vertical rotation (degrees)
        E_WABV_YAW                   = 11, // Head/aim horizontal rotation (degrees)
        E_WABV_ATTACK_DIRECTION      = 12, // Melee attack direction
        E_WABV_STRENGTH              = 13, // Attack strength
        E_WABV_SITU_HEIGHT           = 14, // Situational height
        E_WABV_SITU_TWIST            = 15, // Situational twist
        E_WABV_PED_STYLE             = 16, // Pedestrian style
        E_WABV_PED_AGE               = 17, // Pedestrian age
        E_WABV_PED_WEIGHT            = 18, // Pedestrian weight
        E_WABV_PLAYBACK_RATE         = 19, // Animation playback rate
        E_WABV_RELOAD_PLAYBACK_RATE  = 20, // Reload animation playback rate
        E_WABV_WATER_DEPTH           = 21, // Water depth (affects wading animation)
        E_WABV_CAR_SEAT_HEIGHT       = 22, // Car seat height for IK
        E_WABV_CAR_DOOR_WIDTH        = 23, // Car door width for enter/exit
        E_WABV_MOVE_DESIRED_SPEED    = 24, // Desired movement speed
        E_WABV_INTENSITY             = 25, // Animation intensity
        E_WABV_CAR_LEAN              = 26, // Vehicle leaning
        E_WABV_ATTACK_ANGLE          = 27, // Melee attack angle
        E_WABV_CARJACK_PLAYBACK_RATE = 28, // Carjack animation rate
        E_WABV_ENEMY_FACING          = 29, // Enemy facing direction
        E_WABV_EYE_PITCH             = 30, // Eye vertical rotation (independent of head)
        E_WABV_EYE_YAW               = 31, // Eye horizontal rotation (independent of head)
        E_WABV_FALLOVER_DIRECTION    = 32, // Fall over direction
        E_WABV_LOD_DISTANCE_SQUARE   = 33, // LOD distance squared
        E_WABV_MOVE_SPEED_MULTIPLIER = 34, // Movement speed multiplier
        E_WABV_PARAMETRIC_SPEED_Y    = 35, // Parametric speed Y component
        E_WABV_PARAMETRIC_SPEED_X    = 36, // Parametric speed X component
        E_WABV_OBSTACLE_HEIGHT       = 37, // Obstacle height for traversal
        E_WABV_MOVE_DIR              = 38, // Movement direction

        E_WABV_COUNT                 = 39
    };
} // namespace SDK::ue::game::anim
