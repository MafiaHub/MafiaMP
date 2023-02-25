#include "c_player_teleport_module.h"

#include <utils/hooking/hooking.h>
#include "patterns.h"

namespace SDK {
    C_PlayerTeleportModule *C_PlayerTeleportModule::GetInstance() {
        return *reinterpret_cast<C_PlayerTeleportModule **>(gPatterns.C_PlayerTeleportModule__Instance);
    }

    void C_PlayerTeleportModule::TeleportPlayer(ue::C_CntPtr<uintptr_t> &syncObject, ue::sys::math::C_Vector const &pos, ue::sys::math::C_Vector const &dir, bool unk1, bool unk2, bool unk3, bool unk4) {
        hook::this_call(gPatterns.C_PlayerTeleportModule__TeleportPlayer, this, syncObject, pos, dir, unk1, unk2, unk3, unk4);
    }

    C_PlayerTeleportModule *GetPlayerTeleportModule() {
        return hook::call<C_PlayerTeleportModule *>(hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 83 78 38 00 74 13"));
    }
}
