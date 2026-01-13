#pragma once

#include "src/networking/rpc/game_rpc.h"

namespace MafiaMP::Shared::RPC {
    class HumanShoot final: public Framework::Networking::RPC::IGameRPC<HumanShoot> {
      private:
        glm::vec3 _aimPos;
        glm::vec3 _aimDir;
        bool _unk0;
        bool _unk1;
      public:
        HumanShoot() = default;

        HumanShoot(const glm::vec3 &aimPos, const glm::vec3 &aimDir, bool unk0, bool unk1)
            : _aimPos(aimPos)
            , _aimDir(aimDir)
            , _unk0(unk0)
            , _unk1(unk1) {}

        glm::vec3 GetAimPos() const {
            return _aimPos;
        }

        glm::vec3 GetAimDir() const {
            return _aimDir;
        }

        bool GetUnk0() const {
            return _unk0;
        }

        bool GetUnk1() const {
            return _unk1;
        }

        void Serialize(SLNet::BitStream *bs, bool write) override {
            bs->Serialize(write, _aimPos);
            bs->Serialize(write, _aimDir);
            bs->Serialize(write, _unk0);
            bs->Serialize(write, _unk1);
        }

        bool Valid() const override {
            return true;
        }
    };
} // namespace MafiaMP::Shared::RPC
