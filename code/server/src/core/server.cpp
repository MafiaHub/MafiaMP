#include "server.h"

#include "shared/modules/human_sync.hpp"
#include "shared/modules/vehicle_sync.hpp"

#include "modules/environment.h"
#include "modules/human.h"
#include "modules/vehicle.h"

#include "shared/rpc/chat_message.h"
#include "shared/rpc/environment.h"
#include "shared/rpc/spawn_car.h"

#include <fmt/format.h>

// VERY UGLY
extern std::vector<std::string> TEST_vehiclelist;

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
            BroadcastChatMessage(player, msg);
        });

        SetOnPlayerDisconnectCallback([this](flecs::entity player, uint64_t) {
            const auto st  = player.get<Framework::World::Modules::Base::Streamer>();
            const auto msg = fmt::format("Player {} has left the session!", st->nickname);
            BroadcastChatMessage(player, msg);
        });

        InitRPCs();

        Core::Modules::Human::SetupMessages(this->GetWorldEngine(), net);
        Core::Modules::Vehicle::SetupMessages(this->GetWorldEngine(), net);

        Framework::Logging::GetLogger(FRAMEWORK_INNER_NETWORKING)->info("Networking messages registered!");
    }

    void Server::BroadcastChatMessage(flecs::entity ent, const std::string &msg) {
        FW_SEND_COMPONENT_RPC(Shared::RPC::ChatMessage, msg);
        Framework::Logging::GetLogger("chat")->info(fmt::format("[{}] {}", ent.id(), msg));
    }
    void Server::InitRPCs() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();
        net->RegisterRPC<Shared::RPC::ChatMessage>([this](SLNet::RakNetGUID guid, Shared::RPC::ChatMessage *chatMessage) {
            if (!chatMessage->Valid())
                return;

            const auto ent = GetWorldEngine()->GetEntityByGUID(guid.g);
            if (!ent.is_alive())
                return;

            const auto st  = ent.get<Framework::World::Modules::Base::Streamer>();
            const auto msg = fmt::format("{}: {}", st->nickname, chatMessage->GetText());
            BroadcastChatMessage(ent, msg);
        });

        // spawn car rpc TEST
        net->RegisterRPC<Shared::RPC::SpawnCar>([this](SLNet::RakNetGUID guid, Shared::RPC::SpawnCar *msg) {
            if (!msg->Valid()) {
                return;
            }

            const auto ent = GetWorldEngine()->GetEntityByGUID(guid.g);
            if (!ent.is_alive())
                return;

            if (std::find(TEST_vehiclelist.begin(), TEST_vehiclelist.end(), msg->GetModelName()) == TEST_vehiclelist.end()) {
                return;
            }

            auto t      = ent.get<Framework::World::Modules::Base::Transform>();
            auto carEnt = MafiaMP::Core::Modules::Vehicle::Create(this);

            auto carT = carEnt.get_mut<Framework::World::Modules::Base::Transform>();
            carT->pos = t->pos;
            carT->rot = t->rot;

            auto frame       = carEnt.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = msg->GetModelName();
        });
    }
} // namespace MafiaMP

std::vector<std::string> TEST_vehiclelist = {
    "berkley_810",
    "betting_office",
    "bolt_ace",
    "bolt_ace_pickup",
    "bolt_cooler",
    "bolt_delivery",
    "bolt_delivery_amb",
    "bolt_hearse",
    "bolt_mail",
    "bolt_model_b",
    "bolt_pickup",
    "bolt_truck",
    "bolt_truck_scripted_farm",
    "bolt_v8",
    "brubaker_forte",
    "bulworth_packhard",
    "bulworth_sentry",
    "carrozella_c_series",
    "celeste_mark_5",
    "crazy_horse",
    "culver_airmaster",
    "disorder",
    "eckhart_crusader",
    "eckhart_elite",
    "eckhart_fletcher",
    "falconer_classic",
    "flame_spear",
    "hank_a",
    "haverley_tomahawk_p",
    "houston_coupe",
    "lassiter_v16",
    "lassiter_v16_appolyon",
    "lassiter_v16_roadster",
    "manta_prototype",
    "moto_blackcats",
    "motorcycle_1935",
    "mutagen",
    "parry_bus",
    "samson_drifter",
    "samson_tanker",
    "shubert_e_six_det",
    "shubert_e_six_p",
    "shubert_e_six",
    "shubert_e_six_taxi",
    "shubert_frigate",
    "shubert_six",
    "shubert_six_det",
    "shubert_six_p",
    "shubert_six_taxi",
    "smith_moray",
    "smith_thrower",
    "smith_v12_chicago",
    "smith_v12",
    "trautenberg_sport",
    "waybar_concept",
};
