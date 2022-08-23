#pragma once

#include "src/networking/rpc/rpc.h"

#include <string>

namespace MafiaMP::Shared::RPC {
    class SpawnCar final: public Framework::Networking::RPC::IRPC<SpawnCar> {
      private:
        SLNet::RakString _modelName {};
      public:
        void SetModelName(const std::string& name) {
            _modelName = name.c_str();
        }
        
        std::string GetModelName() {
            return _modelName.C_String();
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _modelName);
        }

        bool Valid() const override {
            return !_modelName.IsEmpty();
        }
    };
} // namespace MafiaMP::Shared::RPC
