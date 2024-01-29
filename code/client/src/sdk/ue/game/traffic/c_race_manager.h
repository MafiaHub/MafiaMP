#pragma once

#include <utils/hooking/hooking.h>

namespace SDK {

    class C_Entity;

    namespace ue::game::traffic {

        class C_RaceManager {
          public:

            // TODO(Greavesy): Current crashes
            void *RegisterEntity(SDK::C_Entity *entity) {
                return hook::this_call<void *>(0x142EFE970, this, entity);
            }

            // TODO(Greavesy): Current crashes
            void *UnregisterEntity(SDK::C_Entity *entity) {
                return hook::this_call<void *>(0x142F10C70, this, entity);
            }
        };
    } // namespace ue::game::traffic
} // namespace SDK
