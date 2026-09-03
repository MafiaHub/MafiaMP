#pragma once

#include <networking/replication/network_entity.h>

#include "shared/modules/human_sync.hpp"

#include <mafianet/string.h>

#include <cstdint>
#include <string>

namespace MafiaMP::Shared::Entities {
    namespace Replication = Framework::Networking::Replication;

    // A replicated human (player avatar or NPC). modelHash carries the spawn profile (skin) used by
    // the client to request the game ped and re-applied when it changes; nickname/playerIndex are
    // spawn-time metadata, and HumanSync::UpdateData carries the per-tick animation/weapon/health/
    // seating state. All of it syncs through the DeltaSerializer — there are no per-property RPCs.
    class HumanEntity : public Replication::NetworkEntity {
      public:
        static constexpr const char *kTypeName = "MafiaMP::Human";

        // Interest radius. The framework's 100 m default is about two seconds of road in a car.
        static constexpr float kStreamRange = 250.0f;

        uint64_t modelHash = 0;
        std::string nickname;
        uint16_t playerIndex = 0xFFFF;
        Modules::HumanSync::UpdateData data {};

        uint64_t GetSpawnProfile() const {
            return modelHash;
        }
        void SetSpawnProfile(uint64_t profile) {
            modelHash = profile;
        }

        void OnSerializeConstruction(Replication::FieldSerializer &fields) override {
            MafiaNet::RakString name(nickname.c_str());
            fields.Field(name);
            fields.Field(playerIndex);
            if (!fields.Writing()) {
                nickname = name.C_String();
            }
        }

        void SerializeFields(Replication::FieldSerializer &fields) override {
            fields.Field(modelHash);
            fields.Field(data);
        }

        // A seated player is meaningless without the car under them.
        Replication::NetworkEntity *GetInterestDependency() override {
            return data.carPassenger.carId != 0 ? ResolveSibling(data.carPassenger.carId) : nullptr;
        }
    };
} // namespace MafiaMP::Shared::Entities
