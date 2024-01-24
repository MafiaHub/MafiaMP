#include "c_player_teleport_module.h"

namespace SDK {
    /**
     * Controls are automatically locked during the transition (loading the world).
     * If you are in a car, your car will also be teleported with you in it
     *
     * @param unk1
     * @param noFadeIn - disable fade in effect
     * @param noFadeEffects - disable fade effects (no fade in, no fade out)
     * @param unk4
     */
    C_PlayerTeleportModule *C_PlayerTeleportModule::TeleportPlayer(ue::C_CntPtr<uintptr_t> &syncObject, ue::sys::math::C_Vector const &pos, ue::sys::math::C_Vector const &dir, bool unk1, bool noFadeIn, bool noFadeEffects, bool unk4) {
        return hook::this_call<C_PlayerTeleportModule *>(gPatterns.C_PlayerTeleportModule__TeleportPlayer, this, syncObject, pos, dir, unk1, noFadeIn, noFadeEffects, unk4);
    }

    C_PlayerTeleportModule *GetPlayerTeleportModule() {
        return hook::call<C_PlayerTeleportModule *>(gPatterns.C_PlayerTeleportModule__GetPlayerTeleportModule);
    }

    C_PlayerTeleportModule *C_PlayerTeleportModule::GetInstance() {
        return *reinterpret_cast<C_PlayerTeleportModule **>(gPatterns.C_PlayerTeleportModule__Instance);
    }
} // namespace SDK
