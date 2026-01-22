#pragma once

#include "c_actor.h"

namespace SDK {
    /**
     * C_ActorVehicle - Base class for vehicle actors
     *
     * Inherits from C_Actor, adds no new data members.
     * Serves as an intermediate class in the hierarchy for type identification.
     *
     * Class Hierarchy:
     *   C_Entity (0xE0) -> C_EntityPos (0xE0) -> C_Actor (0xF8) -> C_ActorVehicle (0xF8)
     *
     * C_Car extends this class and embeds C_Vehicle at offset 0xF8.
     *
     * Size: 0xF8 (248 bytes) - same as C_Actor
     */
    class C_ActorVehicle : public C_Actor {
        // No additional data members - only virtual method overrides for vehicle-specific behavior
    };

    static_assert(sizeof(C_ActorVehicle) == 0xF8, "C_ActorVehicle size mismatch");
} // namespace SDK
