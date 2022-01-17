#include "c_cm_player_alternate.h"

#include <utils/hooking/hooking.h>

namespace SDK {
    namespace ue::game::camera {
        int64_t C_CM_PlayerAlternate::SendCommand(unsigned int modeID, void *data, void *unk) {
            return 0;
        }
    } // namespace ue::game::camera
}
