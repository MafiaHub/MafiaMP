#pragma once

#include <integrations/server/instance.h>

#include "blips.h"

#include <mafianet/types.h>

#include "shared/entities/human_entity.h"

#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>

namespace MafiaMP {
    class Server: public Framework::Integrations::Server::Instance {
      public:
        // Global world state pushed to clients via the SetEnvironment RPC.
        struct Environment {
            std::string weatherSet = "_default_game";
            float dayTimeHours     = 11.0f;
        };

        void PostInit() override;
        void PostScriptInit() override;
        void PostUpdate() override;
        void PreShutdown() override;
        void ModuleRegister(Framework::Scripting::Engine *engine) override;

        void OnPlayerConnect(const Framework::Integrations::Server::PlayerConnectionData &info) override;
        void OnPlayerDisconnect(MafiaNet::PeerGuid guid) override;
        void OnChatMessage(uint64_t senderId, const std::string &text) override;
        void OnChatCommand(uint64_t senderId, const std::string &text, const std::string &command, const std::vector<std::string> &args) override;
        v8::Local<v8::Value> WrapScriptPlayer(v8::Isolate *isolate, uint64_t networkId) override;

        const Environment &GetEnvironment() const {
            return _environment;
        }
        void SetWeatherSet(const std::string &weatherSet);
        void SetDayTimeHours(float dayTimeHours);
        // Send the current environment to one system (UNASSIGNED = broadcast to all).
        void SendEnvironment(MafiaNet::RakNetGUID target, bool broadcast) const;

        Core::Blips &GetBlips() {
            return _blips;
        }

        static inline Server *_serverRef = nullptr;

      private:
        // Seats each player's human where its replicated carPassenger says, and hands each vehicle
        // to its driver's client, or back to the server when nobody drives.
        void SyncVehicleSeats();

        void InitNetworkingMessages();
        void InitRPCs();

        Environment _environment {};

        // Vehicles with a seat taken, the only ones SyncVehicleSeats revisits to release their seats
        // and owner once nobody claims them, rather than walking every entity each tick.
        std::unordered_set<uint64_t> _seatedVehicles;
        // SyncVehicleSeats scratch, kept so its storage is reused from tick to tick.
        std::vector<Shared::Entities::HumanEntity *> _seatClaimants;
        Core::Blips _blips;
    };
} // namespace MafiaMP
