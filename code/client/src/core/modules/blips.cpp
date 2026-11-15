#include "blips.h"

#include "core/modules/human.h"
#include "core/modules/vehicle.h"
#include "shared/rpc/ids.h"

#include "sdk/entities/c_actor.h"
#include "sdk/mafia/ui/navigation/c_navigation.h"
#include "sdk/mafia/ui/navigation/s_entity.h"
#include "sdk/ue/c_string.h"
#include "sdk/ue/sys/math/c_vector.h"
#include "sdk/ue/sys/utils/c_hash_name.h"

#include <core_modules.h>
#include <networking/network_peer.h>
#include <networking/replication/replication_manager.h>

#include <mafianet/bit_stream.h>
#include <mafianet/string.h>

#include <glm/glm.hpp>

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace MafiaMP::Core::Modules::Blips {
    namespace {
        // Drawn in place of a vehicle icon wherever the vehicle's own icon cannot be shown.
        constexpr const char *kVehicleFallbackIcon = "freeride-generic";
        constexpr auto kUpdateInterval             = std::chrono::milliseconds(250);
        // A blip following a streamed-in entity is moved once the entity is this far from its icon.
        constexpr float kFollowThreshold = 1.0f;

        struct Entry {
            // The game icon name, as the server sent it.
            std::string icon;
            // Where the blip is: where the server last put it, or where the followed entity was last
            // seen here while it was streamed in.
            glm::vec3 position {};
            // Entity the blip follows, by NetworkID; 0 for none.
            uint64_t attachedTo = 0;
            // The position icon the mod registered, 0 until it is; and where it is drawn.
            uint32_t navId = 0;
            glm::vec3 drawnAt {};
            // The car whose own vehicle icon draws the blip, while one does.
            SDK::C_Car *vehicleIconCar = nullptr;
        };

        // A vehicle icon the mod registered on a car. The game keeps one navigation entity per scene
        // object, so the blips drawn with the same icon on one car share it.
        struct VehicleIcon {
            std::string icon;
            uint32_t navId = 0;
            int users      = 0;
        };

        std::unordered_map<uint32_t, Entry> gEntries;
        std::unordered_map<SDK::C_Car *, VehicleIcon> gVehicleIcons;
        // The blip the server routes this player to, 0 for none; and the icon the mod selected for it.
        uint32_t gRouteId     = 0;
        uint32_t gRoutedNavId = 0;
        std::chrono::steady_clock::time_point gLastUpdate {};

        bool IsVehicleIcon(const std::string &icon) {
            return icon == "player_car" || icon == "police_car" || icon == "taxi_car" || icon == "enemy";
        }

        SDK::ue::sys::math::C_Vector ToVector(const glm::vec3 &position) {
            return SDK::ue::sys::math::C_Vector(position.x, position.y, position.z);
        }

        // The game actor of the entity a blip follows, while that entity is streamed in here.
        SDK::C_Actor *FollowedActor(const Entry &entry) {
            if (entry.attachedTo == 0) {
                return nullptr;
            }
            if (auto *vehicle = Vehicle::GetByNetworkId(entry.attachedTo)) {
                return vehicle->car;
            }
            auto *repl  = Framework::CoreModules::GetReplication();
            auto *human = repl ? repl->GetEntity<Human>(entry.attachedTo) : nullptr;
            return human ? human->human : nullptr;
        }

        void ReleaseVehicleIcon(Entry &entry, SDK::mafia::ui::navigation::C_Navigation *navigation) {
            if (!entry.vehicleIconCar) {
                return;
            }
            const auto it = gVehicleIcons.find(entry.vehicleIconCar);
            if (it != gVehicleIcons.end() && --it->second.users == 0) {
                if (navigation) {
                    navigation->UnregisterId(it->second.navId);
                }
                gVehicleIcons.erase(it);
            }
            entry.vehicleIconCar = nullptr;
        }

        void AcquireVehicleIcon(Entry &entry, SDK::C_Car *car, SDK::mafia::ui::navigation::C_Navigation *navigation) {
            auto *sceneObject = car->GetSceneObject();
            if (!sceneObject) {
                return;
            }
            if (const auto it = gVehicleIcons.find(car); it != gVehicleIcons.end()) {
                // One icon per car: a blip with another icon keeps its position icon.
                if (it->second.icon != entry.icon) {
                    return;
                }
                ++it->second.users;
            }
            else {
                // RegisterVehicleEntity takes over whatever icon the game gave the car itself.
                if (navigation->GetEntityByGameEntity(sceneObject)) {
                    return;
                }
                const uint32_t navId = navigation->RegisterVehicleEntity(sceneObject, entry.icon.c_str());
                auto *navEntity      = navId ? navigation->GetEntityById(navId) : nullptr;
                if (!navEntity) {
                    return;
                }
                // Vehicle icons are on the radar only until made visible on the map.
                navEntity->SetMapVisible(true);
                gVehicleIcons.emplace(car, VehicleIcon {entry.icon, navId, 1});
            }
            entry.vehicleIconCar = car;
        }

        uint32_t RegisterPositionIcon(const Entry &entry, const glm::vec3 &position, SDK::mafia::ui::navigation::C_Navigation *navigation) {
            const char *icon = IsVehicleIcon(entry.icon) ? kVehicleFallbackIcon : entry.icon.c_str();
            // What the game's five-argument script RegisterIconPos passes, the icon name as its text.
            const SDK::ue::C_String noString;
            const uint16_t visibility = SDK::mafia::ui::navigation::S_Entity::kRadarVisible | SDK::mafia::ui::navigation::S_Entity::kMapVisible;
            return navigation->RegisterIconPos(ToVector(position), icon, SDK::ue::sys::utils::C_HashName::ComputeHash(icon), true, SDK::ue::sys::utils::C_HashName::ComputeHash("0"), noString, visibility, 0xFFFFFF, "", "");
        }

        void Unregister(Entry &entry, SDK::mafia::ui::navigation::C_Navigation *navigation) {
            ReleaseVehicleIcon(entry, navigation);
            if (entry.navId != 0 && navigation) {
                navigation->UnregisterId(entry.navId);
            }
            entry.navId = 0;
        }

        // Brings the game's icons for a blip in line with its entry. It runs again on every update, so
        // an icon the game dropped, or could not take yet, is registered again.
        void Apply(Entry &entry, SDK::mafia::ui::navigation::C_Navigation *navigation) {
            if (entry.navId != 0 && !navigation->GetEntityById(entry.navId)) {
                entry.navId = 0;
            }
            if (entry.vehicleIconCar) {
                const auto it = gVehicleIcons.find(entry.vehicleIconCar);
                if (it == gVehicleIcons.end() || !navigation->GetEntityById(it->second.navId)) {
                    if (it != gVehicleIcons.end()) {
                        gVehicleIcons.erase(it);
                    }
                    entry.vehicleIconCar = nullptr;
                }
            }

            auto *vehicle      = entry.attachedTo != 0 && IsVehicleIcon(entry.icon) ? Vehicle::GetByNetworkId(entry.attachedTo) : nullptr;
            SDK::C_Car *iconCar = vehicle ? vehicle->car : nullptr;
            if (entry.vehicleIconCar != iconCar) {
                ReleaseVehicleIcon(entry, navigation);
            }
            if (iconCar && !entry.vehicleIconCar) {
                AcquireVehicleIcon(entry, iconCar, navigation);
            }

            // A followed entity streamed in here is where the game has it. The server stops sending its
            // moves meanwhile, so the last position seen here is where the blip stays once it goes.
            auto *actor = FollowedActor(entry);
            if (actor) {
                const SDK::ue::sys::math::C_Vector actorPosition = actor->GetPos();
                entry.position                                   = glm::vec3(actorPosition.x, actorPosition.y, actorPosition.z);
            }

            // Under a vehicle icon the position icon stays registered but hidden, and is only moved
            // once it shows again.
            const bool shown = entry.vehicleIconCar == nullptr;
            if (entry.navId == 0) {
                entry.navId   = RegisterPositionIcon(entry, entry.position, navigation);
                entry.drawnAt = entry.position;
            }
            else if (shown && (actor ? glm::distance(entry.position, entry.drawnAt) >= kFollowThreshold : entry.position != entry.drawnAt)) {
                navigation->ChangeIconTargetToPos(entry.navId, ToVector(entry.position));
                entry.drawnAt = entry.position;
            }

            if (auto *navEntity = entry.navId != 0 ? navigation->GetEntityById(entry.navId) : nullptr) {
                navEntity->SetMapVisible(shown);
                navEntity->SetRadarVisible(shown);
            }
        }

        void ApplyRoute(SDK::mafia::ui::navigation::C_Navigation *navigation) {
            const auto it        = gRouteId != 0 ? gEntries.find(gRouteId) : gEntries.end();
            const uint32_t navId = it != gEntries.end() ? it->second.navId : 0;
            if (navId == gRoutedNavId) {
                return;
            }
            if (navId != 0) {
                auto *navEntity = navigation->GetEntityById(navId);
                if (!navEntity) {
                    return;
                }
                navEntity->SetGPSEnabled(true);
                navigation->SelectObjective(navId, true);
            }
            else if (gRoutedNavId != 0 && navigation->GetSelectedObjectiveId() == gRoutedNavId) {
                // Only the route the mod selected is cleared, not one the player picked since.
                navigation->ClearSelectedObjective(true, true);
            }
            gRoutedNavId = navId;
        }

        // Applies one blip at once rather than at the next update, when the game can take it.
        void Refresh(Entry *entry) {
            auto *navigation = SDK::mafia::ui::navigation::C_Navigation::GetInstance();
            if (!navigation) {
                return;
            }
            if (entry) {
                Apply(*entry, navigation);
            }
            ApplyRoute(navigation);
        }

        // Wire: <id><icon><position><attached entity NetworkID>
        void OnSetBlip(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint32_t id = 0;
            MafiaNet::RakString icon;
            glm::vec3 position {};
            uint64_t attachedTo = 0;
            if (!bs->Read(id) || !bs->Read(icon) || !bs->Read(position) || !bs->Read(attachedTo) || icon.IsEmpty()) {
                return;
            }
            auto &entry = gEntries[id];
            if (entry.icon != icon.C_String()) {
                // The icon is set at registration: a new one takes a new registration.
                Unregister(entry, SDK::mafia::ui::navigation::C_Navigation::GetInstance());
                entry.icon = icon.C_String();
            }
            entry.position   = position;
            entry.attachedTo = attachedTo;
            Refresh(&entry);
        }

        // Wire: <id><position>
        void OnMoveBlip(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint32_t id = 0;
            glm::vec3 position {};
            if (!bs->Read(id) || !bs->Read(position)) {
                return;
            }
            // A move that outran the blip's creation is dropped; the creation carries a position.
            const auto it = gEntries.find(id);
            if (it == gEntries.end()) {
                return;
            }
            it->second.position = position;
            Refresh(&it->second);
        }

        // Wire: <id>
        void OnRemoveBlip(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint32_t id = 0;
            if (!bs->Read(id)) {
                return;
            }
            const auto it = gEntries.find(id);
            if (it == gEntries.end()) {
                return;
            }
            Unregister(it->second, SDK::mafia::ui::navigation::C_Navigation::GetInstance());
            gEntries.erase(it);
            Refresh(nullptr);
        }

        // Wire: <id>, 0 for no route
        void OnSetBlipRoute(MafiaNet::BitStream *bs, MafiaNet::Packet *, void *) {
            uint32_t id = 0;
            if (!bs->Read(id)) {
                return;
            }
            gRouteId = id;
            Refresh(nullptr);
        }
    } // namespace

    void Install() {
        auto *net = Framework::CoreModules::GetNetworkPeer();
        auto *rpc = net->GetRPC();
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kSetBlip), &OnSetBlip, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kMoveBlip), &OnMoveBlip, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kRemoveBlip), &OnRemoveBlip, nullptr, 0);
        rpc->RegisterSlot(net->ResolveIdentifier(Shared::RPC::kSetBlipRoute), &OnSetBlipRoute, nullptr, 0);
    }

    void UpdateAll() {
        const auto now = std::chrono::steady_clock::now();
        if ((gEntries.empty() && gRoutedNavId == 0) || now - gLastUpdate < kUpdateInterval) {
            return;
        }
        gLastUpdate = now;

        auto *navigation = SDK::mafia::ui::navigation::C_Navigation::GetInstance();
        if (!navigation) {
            return;
        }
        for (auto &[id, entry] : gEntries) {
            Apply(entry, navigation);
        }
        ApplyRoute(navigation);
    }

    void Clear() {
        auto *navigation = SDK::mafia::ui::navigation::C_Navigation::GetInstance();
        for (auto &[id, entry] : gEntries) {
            Unregister(entry, navigation);
        }
        if (navigation && gRoutedNavId != 0 && navigation->GetSelectedObjectiveId() == gRoutedNavId) {
            navigation->ClearSelectedObjective(true, true);
        }
        gEntries.clear();
        gVehicleIcons.clear();
        gRouteId     = 0;
        gRoutedNavId = 0;
    }

    void OnCarRemoved(SDK::C_Car *car) {
        auto *navigation = SDK::mafia::ui::navigation::C_Navigation::GetInstance();
        for (auto &[id, entry] : gEntries) {
            if (entry.vehicleIconCar == car) {
                ReleaseVehicleIcon(entry, navigation);
            }
        }
    }
} // namespace MafiaMP::Core::Modules::Blips
