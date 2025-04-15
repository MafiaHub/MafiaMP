#include "e_game_camera_mode_id.h"

namespace SDK {
    namespace ue::game::camera {
        const char *GetGameCameraModeString(E_GameCameraModeID mode) {
            switch (mode) {
            case E_GameCameraModeID::E_GCM_NONE: return "E_GCM_NONE";
            case E_GameCameraModeID::E_GCM_DEBUG: return "E_GCM_DEBUG";
            case E_GameCameraModeID::E_GCM_DEBUG_ALTERNATE: return "E_GCM_DEBUG_ALTERNATE";
            case E_GameCameraModeID::E_GCM_CUTSCENE: return "E_GCM_CUTSCENE";
            case E_GameCameraModeID::E_GCM_DIALOG: return "E_GCM_DIALOG";
            case E_GameCameraModeID::E_GCM_ANIMATED: return "E_GCM_ANIMATED";
            case E_GameCameraModeID::E_GCM_AIDEBUG: return "E_GCM_AIDEBUG";
            case E_GameCameraModeID::E_GCM_MAFIA_ENUMBEGIN: return "E_GCM_MAFIA_ENUMBEGIN";
            case E_GameCameraModeID::E_GCM_MAFIA_LIMITEDCONTROL: return "E_GCM_MAFIA_LIMITEDCONTROL";
            case E_GameCameraModeID::E_GCM_MAFIA_PLAYER: return "E_GCM_MAFIA_PLAYER";
            case E_GameCameraModeID::E_GCM_MAFIA_BONE: return "E_GCM_MAFIA_BONE";
            case E_GameCameraModeID::E_GCM_MAFIA_COVER: return "E_GCM_MAFIA_COVER";
            case E_GameCameraModeID::E_GCM_MAFIA_AIM: return "E_GCM_MAFIA_AIM";
            case E_GameCameraModeID::E_GCM_MAFIA_RAIL: return "E_GCM_MAFIA_RAIL";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_STATIC_BUMPER: return "E_GCM_MAFIA_CAR_STATIC_BUMPER";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_STATIC_WHEEL: return "E_GCM_MAFIA_CAR_STATIC_WHEEL";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_STATIC_HOOD: return "E_GCM_MAFIA_CAR_STATIC_HOOD";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_STATIC_LOOKBACK: return "E_GCM_MAFIA_CAR_STATIC_LOOKBACK";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_DRIVERSHOOTFOLLOW: return "E_GCM_MAFIA_CAR_DRIVERSHOOTFOLLOW";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_PASSENGERSHOOT: return "E_GCM_MAFIA_CAR_PASSENGERSHOOT";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_CARGOSPACESHOOT: return "E_GCM_MAFIA_CAR_CARGOSPACESHOOT";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_CARGOSPACELIMITED: return "E_GCM_MAFIA_CAR_CARGOSPACELIMITED";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_FOLLOW: return "E_GCM_MAFIA_CAR_FOLLOW";
            case E_GameCameraModeID::E_GCM_MAFIA_TRUCK_FOLLOW: return "E_GCM_MAFIA_TRUCK_FOLLOW";
            case E_GameCameraModeID::E_GCM_MAFIA_MOTORCYCLE_FOLLOW: return "E_GCM_MAFIA_MOTORCYCLE_FOLLOW";
            case E_GameCameraModeID::E_GCM_MAFIA_MOTORCYCLE_RIDER: return "E_GCM_MAFIA_MOTORCYCLE_RIDER";
            case E_GameCameraModeID::E_GCM_MAFIA_MOTORCYCLE_LOOKBACK: return "E_GCM_MAFIA_MOTORCYCLE_LOOKBACK";
            case E_GameCameraModeID::E_GCM_MAFIA_TRAIN_INTERNAL_FOLLOW: return "E_GCM_MAFIA_TRAIN_INTERNAL_FOLLOW";
            case E_GameCameraModeID::E_GCM_MAFIA_TRAIN_EXTERNAL_DOOR: return "E_GCM_MAFIA_TRAIN_EXTERNAL_DOOR";
            case E_GameCameraModeID::E_GCM_MAFIA_MELEE: return "E_GCM_MAFIA_MELEE";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR: return "E_GCM_MAFIA_CAR";
            case E_GameCameraModeID::E_GCM_MAFIA_GARAGE: return "E_GCM_MAFIA_GARAGE";
            case E_GameCameraModeID::E_GCM_SHOP_MENU: return "E_GCM_SHOP_MENU";
            case E_GameCameraModeID::E_GCM_SCRIPT: return "E_GCM_SCRIPT";
            case E_GameCameraModeID::E_GCM_SCRIPTOVER: return "E_GCM_SCRIPTOVER";
            case E_GameCameraModeID::E_GCM_SCRIPTFRAME: return "E_GCM_SCRIPTFRAME";
            case E_GameCameraModeID::E_GCM_STATIC: return "E_GCM_STATIC";
            case E_GameCameraModeID::E_GCM_FPV: return "E_GCM_FPV";
            case E_GameCameraModeID::E_GCM_DEATH: return "E_GCM_DEATH";
            case E_GameCameraModeID::E_GCM_OPEN_AUTOMATE: return "E_GCM_OPEN_AUTOMATE";
            case E_GameCameraModeID::E_GCM_MAFIA_CAR_INITIAL: return "E_GCM_MAFIA_CAR_INITIAL";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT: return "E_GCM_MAFIA_BOAT";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_FOLLOW: return "E_GCM_MAFIA_BOAT_FOLLOW";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_BOW: return "E_GCM_MAFIA_BOAT_BOW";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_GUNWALE: return "E_GCM_MAFIA_BOAT_GUNWALE";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_ROOF: return "E_GCM_MAFIA_BOAT_ROOF";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_LOOKBACK: return "E_GCM_MAFIA_BOAT_LOOKBACK";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_DRIVERSHOOT: return "E_GCM_MAFIA_BOAT_DRIVERSHOOT";
            case E_GameCameraModeID::E_GCM_MAFIA_BOAT_PASSENGERSHOOT: return "E_GCM_MAFIA_BOAT_PASSENGERSHOOT";
            case E_GameCameraModeID::E_GCM_VEHICLE_DEBUG: return "E_GCM_VEHICLE_DEBUG";
            case E_GameCameraModeID::E_GCM_BOAT_DEBUG: return "E_GCM_BOAT_DEBUG";
            case E_GameCameraModeID::E_GCM_TRAP_CAMERA: return "E_GCM_TRAP_CAMERA";
            case E_GameCameraModeID::E_GCM_LADDER: return "E_GCM_LADDER";
            case E_GameCameraModeID::E_GCM_SWIM: return "E_GCM_SWIM";
            case E_GameCameraModeID::E_GCM_LAST: return "E_GCM_LAST";
            default: return "UNKNOWN_CAMERA_MODE";
            }
        }
    }
}
