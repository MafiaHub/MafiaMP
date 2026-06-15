#include <utils/safe_win32.h>

#include "local_player_events.h"

#include "application.h"

#include "modules/vehicle.h"

#include "shared/entities/human_entity.h"
#include "shared/rpc/ids.h"

#include <networking/network_peer.h>

#include <mafianet/BitStream.h>

#include <cstdint>

namespace MafiaMP::Core {
    namespace {
        MafiaNet::RPC4 *RPC() {
            auto *net = gApplication ? gApplication->GetNetworkingEngine()->GetNetworkClient() : nullptr;
            return net ? net->GetRPC() : nullptr;
        }

        uint64_t LocalPlayerNetworkId() {
            auto *lp = gApplication ? gApplication->GetLocalPlayer() : nullptr;
            return lp ? lp->GetNetworkID() : 0;
        }
    } // namespace

    void LocalPlayerEvents::Died() {
        if (auto *rpc = RPC()) {
            // No payload; the server resolves the dying player from the sender.
            MafiaNet::BitStream bs;
            rpc->Signal(Shared::RPC::kHumanDeath, &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
        }
        // TODO: also raise a client scripting event (e.g. "localPlayerDied").
    }

    void LocalPlayerEvents::Shot(const glm::vec3 &aimPos, const glm::vec3 &aimDir, bool unk0, bool unk1) {
        auto *rpc = RPC();
        if (!rpc) {
            return;
        }
        // Wire: <human NetworkID><aimPos><aimDir><unk0><unk1>
        MafiaNet::BitStream bs;
        uint64_t id = LocalPlayerNetworkId();
        bs.Write(id);
        bs.Write(aimPos);
        bs.Write(aimDir);
        bs.Write(unk0);
        bs.Write(unk1);
        rpc->Signal(Shared::RPC::kHumanShoot, &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
    }

    void LocalPlayerEvents::Reloaded(int mode) {
        auto *rpc = RPC();
        if (!rpc) {
            return;
        }
        // Wire: <human NetworkID><mode>
        MafiaNet::BitStream bs;
        uint64_t id = LocalPlayerNetworkId();
        bs.Write(id);
        bs.Write(mode);
        rpc->Signal(Shared::RPC::kHumanReload, &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
    }

    void LocalPlayerEvents::EnteredVehicle(Modules::Vehicle *vehicle, int seatIndex) {
        auto *rpc = RPC();
        if (!rpc || !vehicle) {
            return;
        }
        // Wire: <vehicle NetworkID><seatIndex>
        MafiaNet::BitStream bs;
        uint64_t vehicleId = vehicle->GetNetworkID();
        bs.Write(vehicleId);
        bs.Write(seatIndex);
        rpc->Signal(Shared::RPC::kVehiclePlayerEnter, &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
    }

    void LocalPlayerEvents::LeftVehicle(Modules::Vehicle *vehicle) {
        auto *rpc = RPC();
        if (!rpc || !vehicle) {
            return;
        }
        // Wire: <vehicle NetworkID>
        MafiaNet::BitStream bs;
        uint64_t vehicleId = vehicle->GetNetworkID();
        bs.Write(vehicleId);
        rpc->Signal(Shared::RPC::kVehiclePlayerLeave, &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, MafiaNet::UNASSIGNED_RAKNET_GUID, true, false);
    }
} // namespace MafiaMP::Core
