#pragma once

#include <integrations/server/instance.h>

#include <mafianet/types.h>

#include <cstdint>
#include <map>
#include <string>
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

        // How the city streams for everyone: the state layer each story location loads (e.g.
        // "hotel" -> "during", its mission interior), and whether every door opens.
        struct WorldSettings {
            bool allDoorsOpen = false;
            std::map<std::string, std::string> locationVariants;
        };
        const WorldSettings &GetWorldSettings() const {
            return _worldSettings;
        }
        void SetLocationVariant(const std::string &location, const std::string &variant);
        void SetAllDoorsOpen(bool open);
        void SendWorldSettings(MafiaNet::RakNetGUID target, bool broadcast) const;

        static inline Server *_serverRef = nullptr;

      private:
        void InitNetworkingMessages();
        void InitRPCs();

        Environment _environment {};
        WorldSettings _worldSettings {};
    };
} // namespace MafiaMP
