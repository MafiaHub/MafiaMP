#pragma once

#include <networking/rpc/rpc_identifier.h>

namespace MafiaMP::Shared::RPC {
    // RPC4 identifiers for transient events and commands. Senders Signal() by these names; receivers
    // RegisterSlot() the same name. The argument wire format (the order of BitStream reads/writes) is
    // documented at each handler/send site.
    //
    // Continuous per-entity state (health, skin, vehicle properties, seating) is NOT here: it lives
    // on the replicated entity and syncs via the DeltaSerializer, not via RPC.

    // Human events (relayed from the acting client to others; the first field is the actor NetworkID)
    inline constexpr auto kHumanShoot     = FW_RPC_IDENTIFIER("MafiaMP::HumanShoot");
    inline constexpr auto kHumanReload    = FW_RPC_IDENTIFIER("MafiaMP::HumanReload");
    inline constexpr auto kHumanDeath     = FW_RPC_IDENTIFIER("MafiaMP::HumanDeath");
    inline constexpr auto kHumanAddWeapon = FW_RPC_IDENTIFIER("MafiaMP::HumanAddWeapon");

    // Vehicle events (player resolved from the sender; carries the vehicle NetworkID)
    inline constexpr auto kVehiclePlayerEnter = FW_RPC_IDENTIFIER("MafiaMP::VehiclePlayerEnter");
    inline constexpr auto kVehiclePlayerLeave = FW_RPC_IDENTIFIER("MafiaMP::VehiclePlayerLeave");

    // Commands
    inline constexpr auto kSetEnvironment = FW_RPC_IDENTIFIER("MafiaMP::SetEnvironment");
    inline constexpr auto kSpawnCar       = FW_RPC_IDENTIFIER("MafiaMP::SpawnCar");
} // namespace MafiaMP::Shared::RPC
