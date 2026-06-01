#pragma once

#include <mafianet/BitStream.h>
#include <glm/glm.hpp>

#include <cstdint>

namespace MafiaMP::Shared::RPC {
    // A shot fired by the human identified by entityId (the shooter's NetworkID), relayed to others.
    struct HumanShoot {
        static constexpr const char *kIdentifier = "MafiaMP::HumanShoot";

        uint64_t entityId = 0;
        glm::vec3 aimPos {};
        glm::vec3 aimDir {};
        bool unk0 = false;
        bool unk1 = false;

        void FromParameters(uint64_t id, const glm::vec3 &aimPos_, const glm::vec3 &aimDir_, bool unk0_, bool unk1_) {
            entityId = id;
            aimPos   = aimPos_;
            aimDir   = aimDir_;
            unk0     = unk0_;
            unk1     = unk1_;
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, entityId);
            bs->Serialize(write, aimPos);
            bs->Serialize(write, aimDir);
            bs->Serialize(write, unk0);
            bs->Serialize(write, unk1);
        }
    };
} // namespace MafiaMP::Shared::RPC
