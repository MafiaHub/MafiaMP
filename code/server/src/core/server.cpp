#include "server.h"

#include "shared/modules/human_sync.hpp"
#include "shared/modules/vehicle_sync.hpp"

#include "core/builtins/builtins.h"

#include "modules/environment.h"
#include "modules/human.h"
#include "modules/vehicle.h"

#include "shared/rpc/chat_message.h"
#include "shared/rpc/environment.h"


#include <fmt/format.h>

#include "shared/game_rpc/human/human_changeskin.h"
 namespace MafiaMP {

    void Server::PostInit() {
        _serverRef = this;
        InitNetworkingMessages();

        // Setup ECS modules (sync)
        GetWorldEngine()->GetWorld()->import <Shared::Modules::HumanSync>();
        GetWorldEngine()->GetWorld()->import <Shared::Modules::VehicleSync>();

        // Setup ECS modules
        GetWorldEngine()->GetWorld()->import <Core::Modules::Environment>();
        GetWorldEngine()->GetWorld()->import <Core::Modules::Human>();
        GetWorldEngine()->GetWorld()->import <Core::Modules::Vehicle>();

        // Setup specific components - default values
        auto weather             = GetWorldEngine()->GetWorld()->get_mut<Core::Modules::Environment::Weather>();
        weather->_weatherSetName = "mm_110_omerta_cp_010_cs_cs_park";
        weather->_dayTimeHours   = 11.0f;
    }

    void Server::PostUpdate() {}

    void Server::PreShutdown() {}

    void Server::InitNetworkingMessages() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();

        SetOnPlayerConnectCallback([this, net](flecs::entity player, uint64_t guid) {
            // Create the networked human entity
            Core::Modules::Human::Create(net, player);

            // Broadcast the environment setup
            const auto weather = GetWorldEngine()->GetWorld()->get<Core::Modules::Environment::Weather>();
            FW_SEND_COMPONENT_RPC_TO(Shared::RPC::SetEnvironment, SLNet::RakNetGUID(guid), SLNet::RakString(weather->_weatherSetName.c_str()), weather->_dayTimeHours);

            // Broadcast chat message
            const auto st  = player.get<Framework::World::Modules::Base::Streamer>();
            const auto msg = fmt::format("Player {} has joined the session!", st->nickname);
            BroadcastChatMessage(msg);

            Scripting::Human::EventPlayerConnected(player);
        });

        SetOnPlayerDisconnectCallback([this](flecs::entity player, uint64_t) {
            const auto st  = player.get<Framework::World::Modules::Base::Streamer>();
            const auto msg = fmt::format("Player {} has left the session!", st->nickname);
            BroadcastChatMessage(msg);

            Scripting::Human::EventPlayerDisconnected(player);
        });

        InitRPCs();

        Core::Modules::Human::SetupMessages(this->GetWorldEngine(), net);
        Core::Modules::Vehicle::SetupMessages(this->GetWorldEngine(), net);

        Framework::Logging::GetLogger(FRAMEWORK_INNER_NETWORKING)->info("Networking messages registered!");
    }

    void Server::ModuleRegister(Framework::Scripting::Engines::SDKRegisterWrapper sdk) {
        if (sdk.GetKind() != Framework::Scripting::ENGINE_NODE)
            return;

        const auto nodeSDK = sdk.GetNodeSDK();
        MafiaMP::Scripting::Builtins::Register(nodeSDK->GetIsolate(), nodeSDK->GetModule());
    }

    void Server::BroadcastChatMessage(const std::string &msg) {
        FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, msg);
    }
    void Server::InitRPCs() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();
        net->RegisterRPC<Shared::RPC::ChatMessage>([this](SLNet::RakNetGUID guid, Shared::RPC::ChatMessage *chatMessage) {
            if (!chatMessage->Valid())
                return;

            const auto ent = GetWorldEngine()->GetEntityByGUID(guid.g);
            if (!ent.is_alive())
                return;

            const auto text = chatMessage->GetText();
            if (text[0] == '/') {
                const auto command  = text.substr(1, text.find(' ') - 1);
                const auto argsPart = text.substr(text.find(' ') + 1);
                std::vector<std::string> args;
                std::string arg;
                std::istringstream iss(argsPart);
                while (iss >> arg) {
                    args.push_back(arg);
                }
                Scripting::World::OnChatCommand(ent, text, command, args);
            } else {
                Scripting::World::OnChatMessage(ent, text);
            }

            // const auto st  = ent.get<Framework::World::Modules::Base::Streamer>();
            // const auto msg = fmt::format("{}: {}", st->nickname, chatMessage->GetText());
            // BroadcastChatMessage(ent, msg);
        });

        // test until we do it via nodejs builtins
        net->RegisterGameRPC<Shared::RPC::HumanChangeSkin>([this](SLNet::RakNetGUID guid, Shared::RPC::HumanChangeSkin *msg) {
            if (!msg->Valid())
                return;

            const auto ent = GetWorldEngine()->GetEntityByGUID(guid.g);
            if (!ent.is_alive())
                return;

            auto frame = ent.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelHash = msg->GetSpawnProfile();
            FW_SEND_SERVER_COMPONENT_GAME_RPC(Shared::RPC::HumanChangeSkin, ent, msg->GetSpawnProfile());
        });
    }
} // namespace MafiaMP
