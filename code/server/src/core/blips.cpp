#include "blips.h"

#include "shared/entities/human_entity.h"
#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <networking/replication/replication_manager.h>

#include <mafianet/bit_stream.h>
#include <mafianet/string.h>

namespace MafiaMP::Core {
    namespace {
        // A followed entity is checked this often, and its blip moved once it is this far away.
        constexpr auto kUpdateInterval = std::chrono::milliseconds(250);
        constexpr float kMoveThreshold = 2.0f;

        Framework::Networking::Replication::NetworkEntity *ResolveEntity(uint64_t networkId) {
            auto *repl = Framework::CoreModules::GetReplication();
            return repl ? repl->GetEntityByNetworkID(networkId) : nullptr;
        }
    } // namespace

    uint32_t Blips::Create(const glm::vec3 &position, const std::string &icon, uint64_t visibleTo) {
        const uint32_t id = _nextId++;
        auto &blip        = _blips[id];
        blip.position     = position;
        blip.icon         = icon;
        blip.visibleTo    = visibleTo;
        Send(id, blip);
        return id;
    }

    void Blips::Destroy(uint32_t id) {
        const auto it = _blips.find(id);
        if (it == _blips.end()) {
            return;
        }
        SendRemove(id, it->second);
        _blips.erase(it);
        // The clients drop the route with the blip.
        std::erase_if(_routes, [id](const auto &route) {
            return route.second == id;
        });
    }

    const Blips::Blip *Blips::Get(uint32_t id) const {
        const auto it = _blips.find(id);
        return it != _blips.end() ? &it->second : nullptr;
    }

    void Blips::SetPosition(uint32_t id, const glm::vec3 &position) {
        const auto it = _blips.find(id);
        if (it == _blips.end()) {
            return;
        }
        it->second.position   = position;
        it->second.attachedTo = 0;
        Send(id, it->second);
    }

    void Blips::SetIcon(uint32_t id, const std::string &icon) {
        const auto it = _blips.find(id);
        if (it == _blips.end() || it->second.icon == icon) {
            return;
        }
        it->second.icon = icon;
        Send(id, it->second);
    }

    void Blips::AttachTo(uint32_t id, uint64_t entityId) {
        const auto it = _blips.find(id);
        if (it == _blips.end()) {
            return;
        }
        auto &blip      = it->second;
        blip.attachedTo = entityId;
        if (auto *entity = entityId ? ResolveEntity(entityId) : nullptr) {
            blip.position = entity->position;
        }
        Send(id, blip);
    }

    void Blips::SetRoute(uint64_t humanId, uint32_t id) {
        auto *human      = dynamic_cast<Shared::Entities::HumanEntity *>(ResolveEntity(humanId));
        const auto *blip = Get(id);
        if (!human || (id != 0 && (!blip || (blip->visibleTo != 0 && blip->visibleTo != humanId)))) {
            return;
        }
        if (id == 0) {
            _routes.erase(humanId);
        }
        else {
            _routes[humanId] = id;
        }
        SendRoute(MafiaNet::ToGuid(human->ownerGUID), id);
    }

    uint32_t Blips::GetRoute(uint64_t humanId) const {
        const auto it = _routes.find(humanId);
        return it != _routes.end() ? it->second : 0;
    }

    void Blips::SendRoute(MafiaNet::RakNetGUID guid, uint32_t id) const {
        auto *net = Framework::CoreModules::GetNetworkPeer();
        if (!net) {
            return;
        }
        // Wire: <id>, 0 for no route
        MafiaNet::BitStream bs;
        bs.Write(id);
        net->GetRPC()->Signal(net->ResolveIdentifier(Shared::RPC::kSetBlipRoute), &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, guid, false, false);
    }

    void Blips::Update() {
        const auto now = std::chrono::steady_clock::now();
        if (now - _lastUpdate < kUpdateInterval) {
            return;
        }
        _lastUpdate = now;

        for (auto &[id, blip] : _blips) {
            if (blip.attachedTo == 0) {
                continue;
            }
            auto *entity = ResolveEntity(blip.attachedTo);
            if (!entity) {
                // The entity is gone: the blip stays where it was last seen.
                blip.attachedTo = 0;
                Send(id, blip);
                continue;
            }
            blip.position = entity->position;
            if (glm::distance(blip.position, blip.sentPosition) >= kMoveThreshold) {
                Send(id, blip);
            }
        }
    }

    void Blips::SendAll(MafiaNet::RakNetGUID guid, uint64_t humanId) const {
        for (const auto &[id, blip] : _blips) {
            if (blip.visibleTo == 0 || blip.visibleTo == humanId) {
                SendTo(id, blip, guid, false);
            }
        }
        if (const uint32_t route = GetRoute(humanId)) {
            SendRoute(guid, route);
        }
    }

    void Blips::OnHumanRemoved(uint64_t humanId) {
        // The player who alone saw these is gone; the client side went with its session.
        std::erase_if(_blips, [humanId](const auto &entry) {
            return entry.second.visibleTo == humanId;
        });
        _routes.erase(humanId);
    }

    bool Blips::Target(const Blip &blip, MafiaNet::RakNetGUID &guid, bool &broadcast) const {
        if (blip.visibleTo == 0) {
            guid      = MafiaNet::UNASSIGNED_RAKNET_GUID;
            broadcast = true;
            return true;
        }
        auto *human = dynamic_cast<Shared::Entities::HumanEntity *>(ResolveEntity(blip.visibleTo));
        if (!human) {
            return false;
        }
        guid      = MafiaNet::ToGuid(human->ownerGUID);
        broadcast = false;
        return true;
    }

    void Blips::Send(uint32_t id, Blip &blip) const {
        MafiaNet::RakNetGUID guid;
        bool broadcast = false;
        if (Target(blip, guid, broadcast)) {
            SendTo(id, blip, guid, broadcast);
        }
        blip.sentPosition = blip.position;
    }

    void Blips::SendTo(uint32_t id, const Blip &blip, MafiaNet::RakNetGUID guid, bool broadcast) const {
        auto *net = Framework::CoreModules::GetNetworkPeer();
        if (!net) {
            return;
        }
        // Wire: <id><icon><position><attached entity NetworkID>
        MafiaNet::BitStream bs;
        bs.Write(id);
        bs.Write(MafiaNet::RakString(blip.icon.c_str()));
        bs.Write(blip.position);
        bs.Write(blip.attachedTo);
        net->GetRPC()->Signal(net->ResolveIdentifier(Shared::RPC::kSetBlip), &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, guid, broadcast, false);
    }

    void Blips::SendRemove(uint32_t id, const Blip &blip) const {
        auto *net = Framework::CoreModules::GetNetworkPeer();
        MafiaNet::RakNetGUID guid;
        bool broadcast = false;
        if (!net || !Target(blip, guid, broadcast)) {
            return;
        }
        // Wire: <id>
        MafiaNet::BitStream bs;
        bs.Write(id);
        net->GetRPC()->Signal(net->ResolveIdentifier(Shared::RPC::kRemoveBlip), &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, guid, broadcast, false);
    }
} // namespace MafiaMP::Core
