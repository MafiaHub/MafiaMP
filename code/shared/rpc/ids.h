#pragma once

namespace MafiaMP::Shared::RPC {
    // RPC4 identifiers for transient events and commands. Senders Signal() by these names; receivers
    // RegisterSlot() the same name (Signal dispatches to slots — RegisterFunction handlers are only
    // reached by Call()). The argument wire format (the order of BitStream reads/writes) is
    // documented at each handler/send site.
    //
    // Continuous per-entity state (health, skin, vehicle properties, seating) is NOT here: it lives
    // on the replicated entity and syncs via the DeltaSerializer, not via RPC.

    // Human events (relayed from the acting client to others; the first field is the actor NetworkID)
    constexpr const char *kHumanShoot     = "MafiaMP::HumanShoot";
    constexpr const char *kHumanReload    = "MafiaMP::HumanReload";
    constexpr const char *kHumanDeath     = "MafiaMP::HumanDeath";
    constexpr const char *kHumanAddWeapon = "MafiaMP::HumanAddWeapon";

    // Vehicle events (player resolved from the sender; carries the vehicle NetworkID)
    constexpr const char *kVehiclePlayerEnter = "MafiaMP::VehiclePlayerEnter";
    constexpr const char *kVehiclePlayerLeave = "MafiaMP::VehiclePlayerLeave";

    // Commands
    constexpr const char *kChatMessage    = "MafiaMP::ChatMessage";
    constexpr const char *kSetEnvironment = "MafiaMP::SetEnvironment";
    constexpr const char *kSpawnCar       = "MafiaMP::SpawnCar";
} // namespace MafiaMP::Shared::RPC
