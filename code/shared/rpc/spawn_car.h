#pragma once

#include <mafianet/BitStream.h>
#include <mafianet/string.h>

#include <string>

namespace MafiaMP::Shared::RPC {
    struct SpawnCar {
        static constexpr const char *kIdentifier = "MafiaMP::SpawnCar";

        MafiaNet::RakString modelName {};

        void SetModelName(const std::string &name) {
            modelName = name.c_str();
        }

        std::string GetModelName() const {
            return modelName.C_String();
        }

        void Serialize(MafiaNet::BitStream *bs, bool write) {
            bs->Serialize(write, modelName);
        }

        bool Valid() const {
            return !modelName.IsEmpty();
        }
    };
} // namespace MafiaMP::Shared::RPC
