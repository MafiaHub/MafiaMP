#include "server.h"

#include "shared/modules/human_sync.hpp"
#include "shared/modules/vehicle_sync.hpp"

#include "modules/human.h"
#include "modules/vehicle.h"

#include "shared/rpc/spawn_car.h"
#include "shared/rpc/chat_message.h"

#include <fmt/format.h>

// TODO UGLY
void spawnSomeCars(MafiaMP::Server *server);

extern std::vector<std::string> TEST_vehiclelist;

namespace MafiaMP {

    void Server::PostInit() {
        _serverRef = this;
        InitNetworkingMessages();

        // Setup ECS modules (sync)
        GetWorldEngine()->GetWorld()->import<Shared::Modules::HumanSync>();
        GetWorldEngine()->GetWorld()->import<Shared::Modules::VehicleSync>();

        // Setup ECS modules
        GetWorldEngine()->GetWorld()->import<Core::Modules::Human>();
        GetWorldEngine()->GetWorld()->import<Core::Modules::Vehicle>();

        // TODO UGLY
        spawnSomeCars(this);
    }

    void Server::PostUpdate() {}

    void Server::PreShutdown() {}

    void Server::InitNetworkingMessages() {
        const auto net = GetNetworkingEngine()->GetNetworkServer();

        SetOnPlayerConnectCallback([this, net](flecs::entity player, uint64_t) {
            Core::Modules::Human::Create(net, player);
            
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
        Shared::RPC::ChatMessage proxyMsg {};
        proxyMsg.FromParameters(msg);
        GetNetworkingEngine()->GetNetworkServer()->SendRPC(proxyMsg, SLNet::UNASSIGNED_RAKNET_GUID);
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

            const auto st = ent.get<Framework::World::Modules::Base::Streamer>();
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

            auto t = ent.get<Framework::World::Modules::Base::Transform>();
            auto carEnt = MafiaMP::Core::Modules::Vehicle::Create(this);

            auto carT = carEnt.get_mut<Framework::World::Modules::Base::Transform>();
            carT->pos = t->pos;
            carT->rot = t->rot;

            auto frame       = carEnt.get_mut<Framework::World::Modules::Base::Frame>();
            frame->modelName = msg->GetModelName();
        });
    }
} // namespace MafiaMP

// TODO UGLY

void spawnSomeCars(MafiaMP::Server *server) {
    struct CarLocation {
        const char* modelName;
        glm::vec3 pos;
        glm::quat rot;
    };
    static const CarLocation locations[] = {
        {"berkley_810", {-986.406860,-304.061798,2.292042}, {0.706290, 0.006456,-0.004993,-0.707875}},
        {"bolt_ace", {-985.365356,-336.348083,2.892426}, {0.702591, 0.009590,0.005905,-0.711505}},
        {"bolt_ace_pickup", {-986.426086,-343.213989,2.942883}, {0.734178, 0.009719,0.005454,-0.678866}},
        {"bolt_cooler", {-986.180481,-346.679779,2.861760}, {0.699282, 0.007726,0.000273,-0.714804}},
        {"bolt_delivery", {-986.150085,-349.681244,2.773793}, {0.707309, 0.008673,0.002028,-0.706848}},
        {"bolt_delivery_amb", {-986.273193,-352.752472,2.802820}, {0.722713, 0.008347,0.001777,-0.691096}},
        {"bolt_hearse", {-1018.279968,-340.606567,2.919076}, {0.723420, 0.023575,-0.020525,0.689701}},
        {"bolt_mail", {-1015.080139,-344.603149,2.729358}, {0.709010, 0.019996,-0.011678,0.704818}},
        {"bolt_model_b", {-1014.999268,-347.786743,2.713083}, {0.701918, 0.016684,-0.009163,0.712003}},
        {"bolt_pickup", {-1014.623901,-350.718506,2.802622}, {0.688722, 0.015586,-0.009113,0.724800}},
        {"bolt_truck", {-984.092224,-395.968231,3.501376}, {0.716167, 0.003727,0.006030,-0.697893}},
        {"bolt_v8", {-1014.788574,-353.798706,2.868016}, {0.707765, 0.029924,0.002543,0.705809}},
        {"brubaker_forte", {-1111.575073,-235.930466,2.627016}, {0.999993, 0.000999,-0.001624,-0.003262}},
        {"bulworth_packhard", {-1128.774658,-210.558975,2.788571}, {0.018355, -0.000992,0.004058,-0.999823}},
        {"bulworth_sentry", {-1132.614990,-211.015091,2.987224}, {0.005594, 0.003860,-0.007584,0.999948}},
        {"carrozella_c_series", {-1114.119995,-235.950943,2.574988}, {0.999987, 0.000122,-0.001638,0.004916}},
        {"celeste_mark_5", {-1128.145630,-235.091614,2.740970}, {0.999955, 0.004230,0.002423,-0.008165}},
        {"culver_airmaster", {-1131.364136,-234.920700,2.780458}, {0.999437, 0.007040,-0.001867,-0.032764}},
        {"eckhart_crusader", {-1134.742920,-234.785156,2.649705}, {0.999528, 0.007318,0.001145,-0.029805}},
        {"eckhart_elite", {-1138.363647,-234.632172,2.741032}, {0.999839, 0.008441,-0.004551,-0.015151}},
        {"eckhart_fletcher", {-1124.938232,-234.045761,2.648867}, {0.999995, 0.001460,-0.002389,-0.001145}},
        {"falconer_classic", {-1135.854736,-210.904358,2.817213}, {0.002454, -0.007236,0.003241,-0.999966}},
        {"houston_coupe", {-914.322388,-120.731689,3.724135}, {0.722015, -0.010507,-0.006834,0.691764}},
        {"lassiter_v16", {-914.743103,-117.816315,3.732256}, {0.727089, -0.011908,-0.007143,0.686403}},
        {"lassiter_v16_appolyon", {-915.122864,-114.279373,3.828074}, {0.695113, -0.010159,-0.007071,0.718794}},
        {"lassiter_v16_roadster", {-914.985962,-111.232590,3.845985}, {0.718718, -0.011091,-0.002821,0.695208}},
        {"parry_bus", {-962.758606,-65.251747,4.112756}, {0.703505, 0.007003,0.002187,-0.710653}},
        {"samson_drifter", {-915.068787,-108.109604,3.961980}, {0.729244, -0.006566,-0.005744,0.684198}},
        {"samson_tanker", {-963.507813,-59.896549,3.865996}, {0.703396, 0.004940,0.009704,-0.710714}},
        {"shubert_e_six", {-970.537109,-97.723427,3.664779}, {0.999958, -0.004803,0.006743,0.003864}},
        {"shubert_e_six_p", {-973.756897,-98.136681,3.633013}, {0.999966, -0.004319,0.006257,-0.003289}},
        {"shubert_e_six_taxi", {-977.302856,-98.218300,3.608772}, {0.999970, -0.002881,0.004691,-0.005354}},
        {"shubert_frigate", {-980.905090,-97.996140,3.381163}, {0.999725, -0.008196,0.005107,-0.021349}},
        {"shubert_six", {-984.109741,-98.175415,3.511434}, {0.999940, -0.005686,0.005481,-0.007586}},
        {"shubert_six_det", {-986.901978,-98.312843,3.462155}, {0.999794, -0.005663,0.006068,-0.018523}},
        {"shubert_six_p", {-990.067322,-98.177017,3.464381}, {0.999615, -0.005160,0.006077,0.026569}},
        {"shubert_six_taxi", {-993.492859,-98.207672,3.423491}, {0.999974, -0.005231,0.004000,-0.002960}},
        {"smith_moray", {-996.777039,-98.042068,3.426675}, {0.999928, -0.002293,0.010735,0.004858}},
        {"smith_thrower", {-1001.699707,-98.202988,3.406393}, {0.999839, 0.007582,0.005775,0.015234}},
        {"smith_v12", {-1005.234802,-98.821114,3.299397}, {0.999980, -0.001132,0.005176,0.003361}},
        {"smith_v12_chicago", {-1009.044983,-98.970703,3.265859}, {0.999996, -0.002558,0.000828,0.000000}},
        {"trautenberg_sport", {-1012.573914,-98.974365,3.331904}, {0.999934, -0.001642,0.005150,0.010147}},
        {"crazy_horse", {-934.128235,-234.337341,2.938883}, {0.999870, 0.016064,-0.000752,-0.001326}},
        {"disorder", {-975.474548,-236.047623,3.025368}, {0.700711, 0.001802,-0.001995,0.713440}},
        {"flame_spear", {-974.755005,-233.292252,2.886548}, {0.715502, -0.000532,-0.000854,0.698610}},
        {"manta_prototype", {-970.275513,-228.638977,2.805085}, {0.404505, -0.000488,0.000977,0.914535}},
        {"mutagen", {-968.035583,-239.757065,3.011281}, {0.890560, -0.003047,-0.000000,0.454856}},
        {"hank_a", {-876.823364,-179.889084,3.178279}, {0.699958, 0.010274,0.009395,-0.714049}},
        {"shubert_e_six", {-906.617432,-193.383072,2.961988}, {0.020849, -0.034580,0.001879,-0.999183}},
        {"bolt_v8", {-906.344666,-155.046341,3.101451}, {0.015857, 0.039470,-0.007466,0.999067}},
        {"bolt_model_b", {-906.452148,-242.218521,2.937189}, {0.002211, -0.032577,-0.004167,-0.999458}},
        {"shubert_six_det", {-890.745178,-221.919250,2.894908}, {0.999474, -0.003435,0.027849,-0.016274}},
        {"culver_airmaster", {-890.607422,-199.794052,2.993601}, {0.999442, 0.001434,0.033379,0.000244}},
        {"houston_coupe", {-890.751465,-147.323196,3.245617}, {0.999277, -0.011216,0.034156,0.012345}}
    };
    size_t i;
    for(i = 0; i < sizeof(locations) / sizeof(CarLocation); ++i) {
        const CarLocation& loc = locations[i];

        auto e = MafiaMP::Core::Modules::Vehicle::Create(server);

        auto t = e.get_mut<Framework::World::Modules::Base::Transform>();
        t->pos = loc.pos;
        t->rot = loc.rot;

        auto frame = e.get_mut<Framework::World::Modules::Base::Frame>();
        frame->modelName = loc.modelName;
    }
    Framework::Logging::GetLogger("test")->info("[INFO] Spawned {} cars!", i);
}

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
