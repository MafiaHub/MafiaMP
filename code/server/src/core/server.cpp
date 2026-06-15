#include "server.h"

#include "core/builtins/builtins.h"
#include "core/builtins/chat.h"
#include "core/builtins/player.h"
#include "core/builtins/vehicle.h"

#include "shared/entities/human_entity.h"
#include "shared/entities/register_entities.h"
#include "shared/entities/vehicle_entity.h"
#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_server.h>
#include <networking/replication/replication_manager.h>

#include <scripting/node_engine.h>
#include <scripting/resource/resource_manager.h>

#include <mafianet/BitStream.h>
#include <mafianet/string.h>
#include <utils/optional.h>
#include <v8pp/convert.hpp>

#include <glm/glm.hpp>

#include <sstream>
#include <string>
#include <vector>

namespace MafiaMP {
    namespace {
        // Default human skin/spawn-profile until the game assigns one.
        constexpr uint64_t kDefaultSkin = 335218123840277515ULL;

        Framework::Networking::Replication::ReplicationManager *Replication() {
            return Framework::CoreModules::GetReplication();
        }

        // Resolve the human a connection looks through (its avatar).
        Shared::Entities::HumanEntity *ViewerHuman(MafiaNet::PeerGuid guid) {
            auto *repl = Replication();
            return repl ? dynamic_cast<Shared::Entities::HumanEntity *>(repl->GetViewer(guid)) : nullptr;
        }

        template <typename T>
        T *ResolveEntity(uint64_t networkId) {
            auto *repl = Replication();
            return repl ? dynamic_cast<T *>(repl->GetEntityByNetworkID(networkId)) : nullptr;
        }

        // --- Raw RPC4 event handlers ---

        // Wire: <shooter NetworkID><aimPos><aimDir><unk0><unk1>. Relayed to everyone but the shooter.
        void OnHumanShoot(MafiaNet::BitStream *bs, MafiaNet::Packet *packet, void *) {
            uint64_t shooterId = 0;
            glm::vec3 aimPos {}, aimDir {};
            bool unk0 = false, unk1 = false;
            bs->Read(shooterId);
            bs->Read(aimPos);
            bs->Read(aimDir);
            bs->Read(unk0);
            bs->Read(unk1);

            auto *net = static_cast<Framework::Networking::NetworkServer *>(Framework::CoreModules::GetNetworkPeer());
            if (!net) {
                return;
            }
            MafiaNet::BitStream out;
            out.Write(shooterId);
            out.Write(aimPos);
            out.Write(aimDir);
            out.Write(unk0);
            out.Write(unk1);
            net->SignalExcept(Shared::RPC::kHumanShoot, out, packet->guid);
        }

        // Wire: <shooter NetworkID><unk0>. Relayed to everyone but the shooter.
        void OnHumanReload(MafiaNet::BitStream *bs, MafiaNet::Packet *packet, void *) {
            uint64_t shooterId = 0;
            int unk0           = 0;
            bs->Read(shooterId);
            bs->Read(unk0);

            auto *net = static_cast<Framework::Networking::NetworkServer *>(Framework::CoreModules::GetNetworkPeer());
            if (!net) {
                return;
            }
            MafiaNet::BitStream out;
            out.Write(shooterId);
            out.Write(unk0);
            net->SignalExcept(Shared::RPC::kHumanReload, out, packet->guid);
        }

        // Wire: empty. The dying player is resolved from the sender.
        void OnHumanDeath(MafiaNet::BitStream *, MafiaNet::Packet *packet, void *) {
            if (auto *human = ViewerHuman(MafiaNet::ToPeerGuid(packet->guid))) {
                Scripting::Player::EventPlayerDied(human->GetNetworkID());
            }
        }

        // Wire: <vehicle NetworkID><seatIndex>.
        void OnVehiclePlayerEnter(MafiaNet::BitStream *bs, MafiaNet::Packet *packet, void *) {
            uint64_t vehicleId = 0;
            int seatIndex      = 0;
            bs->Read(vehicleId);
            bs->Read(seatIndex);

            auto *player  = ViewerHuman(MafiaNet::ToPeerGuid(packet->guid));
            auto *vehicle = ResolveEntity<Shared::Entities::VehicleEntity>(vehicleId);
            if (!player || !vehicle) {
                return;
            }
            if (seatIndex >= 0 && seatIndex < Shared::Entities::VehicleEntity::kMaxSeats) {
                vehicle->seats[seatIndex] = player->GetNetworkID();
                if (seatIndex == 0) {
                    // The driver's client becomes authoritative for the vehicle.
                    vehicle->SetOwner(MafiaNet::ToPeerGuid(packet->guid));
                }
            }
            Scripting::Vehicle::EventVehiclePlayerEnter(vehicleId, player->GetNetworkID(), seatIndex);
        }

        // Wire: <vehicle NetworkID>.
        void OnVehiclePlayerLeave(MafiaNet::BitStream *bs, MafiaNet::Packet *packet, void *) {
            uint64_t vehicleId = 0;
            bs->Read(vehicleId);

            auto *player  = ViewerHuman(MafiaNet::ToPeerGuid(packet->guid));
            auto *vehicle = ResolveEntity<Shared::Entities::VehicleEntity>(vehicleId);
            if (!player || !vehicle) {
                return;
            }
            const uint64_t playerId = player->GetNetworkID();
            for (int i = 0; i < Shared::Entities::VehicleEntity::kMaxSeats; ++i) {
                if (vehicle->seats[i] == playerId) {
                    vehicle->seats[i] = 0;
                    if (i == 0) {
                        vehicle->SetOwner(MafiaNet::UNASSIGNED_PEER_GUID); // back to the server
                    }
                }
            }
            Scripting::Vehicle::EventVehiclePlayerLeave(vehicleId, playerId);
        }

        // Wire: <modelName>.
        void OnSpawnCar(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            MafiaNet::RakString modelName;
            if (!bs->Read(modelName)) {
                return;
            }
            auto *engine = Replication();
            if (!engine) {
                return;
            }
            if (auto *vehicle = dynamic_cast<Shared::Entities::VehicleEntity *>(engine->CreateEntity(Shared::Entities::VehicleTypeId()))) {
                vehicle->modelName = modelName.C_String();
            }
        }
    } // namespace

    void Server::PostInit() {
        _serverRef = this;

        // Register the replicated entity types so the server can construct them.
        Shared::Entities::RegisterEntities();

        InitNetworkingMessages();
    }

    void Server::PostUpdate() {}

    void Server::PreShutdown() {}

    // Bridge framework chat into the gamemode's scripting events (the framework parses and resolves
    // the sender; we surface it to JS with the mod's Player handle).
    void Server::OnChatMessage(uint64_t senderId, const std::string &text) {
        Scripting::Chat::EventChatMessage(senderId, text);
    }

    void Server::OnChatCommand(uint64_t senderId, const std::string &text, const std::string &command, const std::vector<std::string> &args) {
        Scripting::Chat::EventChatCommand(senderId, text, command, args);
    }

    void Server::SetWeatherSet(const std::string &weatherSet) {
        _environment.weatherSet = weatherSet;
        SendEnvironment(MafiaNet::UNASSIGNED_RAKNET_GUID, true);
    }

    void Server::SetDayTimeHours(float dayTimeHours) {
        _environment.dayTimeHours = dayTimeHours;
        SendEnvironment(MafiaNet::UNASSIGNED_RAKNET_GUID, true);
    }

    void Server::SendEnvironment(MafiaNet::RakNetGUID target, bool broadcast) const {
        auto *net = GetNetworkingEngine()->GetNetworkServer();
        if (!net) {
            return;
        }
        // Wire: <optional weatherSet><optional dayTimeHours>
        MafiaNet::BitStream bs;
        Framework::Utils::Optional<MafiaNet::RakString> weather(MafiaNet::RakString(_environment.weatherSet.c_str()));
        Framework::Utils::Optional<float> dayTime(_environment.dayTimeHours);
        weather.Serialize(&bs, true);
        dayTime.Serialize(&bs, true);
        net->GetRPC()->Signal(Shared::RPC::kSetEnvironment, &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, broadcast, false);
    }

    void Server::InitNetworkingMessages() {
        InitRPCs();
    }

    void Server::OnPlayerConnect(const Framework::Integrations::Server::PlayerConnectionData &info) {
        auto *engine = Replication();
        auto *net    = GetNetworkingEngine()->GetNetworkServer();
        auto *repl   = net ? net->GetReplicationManager() : nullptr;
        if (!engine || !repl) {
            return;
        }

        // Create the player's avatar, own it, populate its spawn metadata, and make it this
        // connection's viewer.
        auto *human = dynamic_cast<Shared::Entities::HumanEntity *>(engine->CreateEntity(Shared::Entities::HumanTypeId()));
        if (!human) {
            return;
        }
        human->ownerGUID   = info.guid;
        human->modelHash   = kDefaultSkin;
        human->nickname    = info.nickname;
        human->playerIndex = info.playerIndex;
        repl->SetViewer(info.guid, human);

        SendEnvironment(MafiaNet::ToGuid(info.guid), false);
        Scripting::Player::EventPlayerConnected(human->GetNetworkID());
    }

    void Server::OnPlayerDisconnect(MafiaNet::PeerGuid guid) {
        if (auto *human = ViewerHuman(guid)) {
            Scripting::Player::EventPlayerDisconnected(human->GetNetworkID());
        }
    }

    void Server::InitRPCs() {
        auto *rpc = GetNetworkingEngine()->GetNetworkServer()->GetRPC();
        // Gameplay RPCs received from clients.
        rpc->RegisterSlot(Shared::RPC::kHumanShoot, &OnHumanShoot, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kHumanReload, &OnHumanReload, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kHumanDeath, &OnHumanDeath, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kVehiclePlayerEnter, &OnVehiclePlayerEnter, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kVehiclePlayerLeave, &OnVehiclePlayerLeave, nullptr, 0);
        rpc->RegisterSlot(Shared::RPC::kSpawnCar, &OnSpawnCar, nullptr, 0);
    }

    void Server::ModuleRegister(Framework::Scripting::Engine *engine) {
        auto *nodeEngine = dynamic_cast<Framework::Scripting::NodeEngine *>(engine);
        if (!nodeEngine) {
            return;
        }

        v8::Isolate *isolate = nodeEngine->GetIsolate();
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);
        v8::Local<v8::Context> context = nodeEngine->GetContext();
        v8::Context::Scope contextScope(context);

        v8::Local<v8::Object> global = context->Global();
        v8::Local<v8::Value> frameworkVal;
        if (global->Get(context, v8pp::to_v8(isolate, "Framework")).ToLocal(&frameworkVal) && frameworkVal->IsObject()) {
            v8::Local<v8::Object> frameworkObj = frameworkVal.As<v8::Object>();
            MafiaMP::Scripting::Builtins::Register(isolate, global, frameworkObj);
        }
    }
} // namespace MafiaMP
