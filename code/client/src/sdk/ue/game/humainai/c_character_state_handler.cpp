#include "c_character_state_handler.h"

#include <utils/hooking/hooking.h>
#include "../../../entities/c_entity.h"
#include "../../../entities/c_human_2.h"
#include "../../../patterns.h"

#include "c_character_controller.h"

namespace SDK {
    namespace ue::game::humanai {
        const char *C_CharacterStateHandler::GetStateHandlerTypeName(E_State_Handler_Type type) {
            switch (type) {
            case E_SHT_NONE: return "None";
            case E_SHT_DEATH: return "Death";
            case E_SHT_MOVE: return "Move";
            case E_SHT_STAND: return "Stand";
            case E_SHT_MOVESTAND: return "MoveStand";
            case E_SHT_CLIMB: return "Climb";
            case E_SHT_COVER: return "Cover";
            case E_SHT_CAR: return "Car";
            case E_SHT_MELEE: return "Melee";
            case E_SHT_ACTION: return "Action";
            case E_SHT_WEAPON: return "Weapon";
            case E_SHT_GRENADE: return "Grenade";
            case E_SHT_AIM: return "Aim";
            case E_SHT_AWARENESS: return "Awareness";
            case E_SHT_INJURY: return "Injury";
            case E_SHT_SPEECH: return "Speech";
            case E_SHT_PLAYANIM: return "PlayAnim";
            case E_SHT_PLAYOVERLAYANIM: return "PlayOverlayAnim";
            case E_SHT_LOOKAT: return "LookAt";
            case E_SHT_SWIM: return "Swim";
            case E_SHT_BOAT: return "Boat";
            case E_SHT_PREVIEW: return "Preview";
            case E_SHT_FALL: return "Fall";
            case E_SHT_LOCKPULL: return "LockPull";
            case E_SHT_PLAYERREACTION: return "PlayerReaction";
            case E_SHT_ACTION_OVERLAY: return "ActionOverlay";
            case E_SHT_TRAIN: return "Train";
            case E_SHT_MOTORCYCLE: return "Motorcycle";
            case E_SHT_DOWNED: return "Downed";
            case E_SHT_TURRET: return "Turret";
            case E_SHT_INVESTIGATE: return "Investigate";
            case E_SHT_LAST: return "Last";
            default: return "Unknown";
            }
        }
    } // namespace ue::game::humanai
}
