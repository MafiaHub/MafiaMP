#include "blips.h"

#include <fmt/format.h>

#include "../../core/application.h"
#include "../../core/modules/vehicle.h"

#include "sdk/entities/c_car.h"
#include "sdk/mafia/ui/navigation/c_navigation.h"

#include <chrono>
#include <unordered_map>

// The position icons go through the game's Lua navigation bindings, which take the game's own
// strings and vectors; their navigation ids stay in a Lua table keyed by our blip id. Vehicle icons
// are registered natively on the car.
namespace MafiaMP::Game::Helpers::Blips {
    namespace {
        // Position icon of a blip whose vehicle icon cannot be shown.
        constexpr const char *kVehicleFallbackIcon = "freeride-generic";
        constexpr auto kUpdateInterval             = std::chrono::milliseconds(250);

        struct Entry {
            std::string icon;
            uint64_t attachedTo   = 0;
            SDK::C_Car *car       = nullptr;
            uint32_t vehicleNavId = 0;
        };

        std::unordered_map<uint32_t, Entry> gEntries;
        uint32_t gRouteId = 0;
        std::chrono::steady_clock::time_point gLastUpdate {};

        bool IsIconName(const std::string &icon) {
            return !icon.empty() && icon.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_-") == std::string::npos;
        }

        bool IsVehicleIcon(const std::string &icon) {
            return icon == "player_car" || icon == "police_car" || icon == "taxi_car" || icon == "enemy";
        }

        void Execute(const std::string &code) {
            if (auto *app = Core::gApplication.get(); app && app->GetLuaVM()) {
                app->GetLuaVM()->ExecuteString(code.c_str());
            }
        }

        void SetPositionIconShown(uint32_t id, bool shown) {
            Execute(fmt::format(R"(local e = MafiaMPBlips and MafiaMPBlips[{0}]
if e then
    game.navigation:SetIconMapVisibilityByNavId(e.nav, {1})
    game.navigation:SetIconRadarVisibilityByNavId(e.nav, {1})
end)",
                id, shown));
        }

        void UnregisterVehicleIcon(uint32_t id, Entry &entry) {
            if (entry.vehicleNavId == 0) {
                return;
            }
            if (auto *navigation = SDK::mafia::ui::navigation::C_Navigation::GetInstance()) {
                navigation->UnregisterId(entry.vehicleNavId);
            }
            entry.vehicleNavId = 0;
            entry.car          = nullptr;
            SetPositionIconShown(id, true);
        }

        SDK::C_Car *FollowedCar(const Entry &entry) {
            if (entry.attachedTo == 0 || !IsVehicleIcon(entry.icon)) {
                return nullptr;
            }
            auto *vehicle = Core::Modules::Vehicle::GetByNetworkId(entry.attachedTo);
            return vehicle ? vehicle->car : nullptr;
        }
    } // namespace

    void Set(uint32_t id, const std::string &icon, const glm::vec3 &position, uint64_t attachedTo) {
        if (!IsIconName(icon)) {
            return;
        }
        auto &entry = gEntries[id];
        if (entry.icon != icon || entry.attachedTo != attachedTo) {
            UnregisterVehicleIcon(id, entry);
        }
        entry.icon       = icon;
        entry.attachedTo = attachedTo;

        // A changed icon needs a new registration; otherwise the existing one is moved.
        const std::string positionIcon = IsVehicleIcon(icon) ? kVehicleFallbackIcon : icon;
        Execute(fmt::format(R"(local b = MafiaMPBlips or {{}}
MafiaMPBlips = b
local e = b[{0}]
if e and e.icon ~= "{1}" then
    game.navigation:UnregisterId(e.nav)
    e = nil
end
if e then
    game.navigation:ChangeIconTargetToPos(e.nav, Math:newVector({2:.2f}, {3:.2f}, {4:.2f}))
else
    e = {{icon = "{1}", nav = game.navigation:RegisterIconPos({2:.2f}, {3:.2f}, "{1}", "{1}", true)}}
    b[{0}] = e
end
game.navigation:SetIconMapVisibilityByNavId(e.nav, {5})
game.navigation:SetIconRadarVisibilityByNavId(e.nav, {5})
if MafiaMPRoute == {0} then
    game.navigation:SetIconGPSEnabledByNavId(e.nav, true)
    game.navigation:SelectIcon(e.nav)
end)",
            id, positionIcon, position.x, position.y, position.z, entry.vehicleNavId == 0));
    }

    void Remove(uint32_t id) {
        if (const auto it = gEntries.find(id); it != gEntries.end()) {
            UnregisterVehicleIcon(id, it->second);
            gEntries.erase(it);
        }
        if (gRouteId == id) {
            gRouteId = 0;
            Execute("MafiaMPRoute = 0\ngame.navigation:ClearSelectedIcon()");
        }
        Execute(fmt::format(R"(local e = MafiaMPBlips and MafiaMPBlips[{0}]
if e then
    game.navigation:UnregisterId(e.nav)
    MafiaMPBlips[{0}] = nil
end)",
            id));
    }

    void Clear() {
        for (auto &[id, entry] : gEntries) {
            UnregisterVehicleIcon(id, entry);
        }
        gEntries.clear();
        gRouteId = 0;
        Execute(R"(for _, e in pairs(MafiaMPBlips or {}) do
    game.navigation:UnregisterId(e.nav)
end
MafiaMPBlips = {}
MafiaMPRoute = 0)");
    }

    void SetRoute(uint32_t id) {
        gRouteId = id;
        Execute(fmt::format(R"(MafiaMPRoute = {0}
local e = MafiaMPBlips and MafiaMPBlips[{0}]
if e then
    game.navigation:SetIconGPSEnabledByNavId(e.nav, true)
    game.navigation:SelectIcon(e.nav)
else
    game.navigation:ClearSelectedIcon()
end)",
            id));
    }

    void Update() {
        const auto now = std::chrono::steady_clock::now();
        if (now - gLastUpdate < kUpdateInterval) {
            return;
        }
        gLastUpdate = now;

        auto *navigation = SDK::mafia::ui::navigation::C_Navigation::GetInstance();
        if (!navigation) {
            return;
        }
        for (auto &[id, entry] : gEntries) {
            auto *car = FollowedCar(entry);
            if (entry.vehicleNavId != 0 && car != entry.car) {
                UnregisterVehicleIcon(id, entry);
            }
            if (car && entry.vehicleNavId == 0 && car->GetSceneObject()) {
                entry.vehicleNavId = navigation->RegisterVehicleEntity(car->GetSceneObject(), entry.icon.c_str());
                if (entry.vehicleNavId != 0) {
                    entry.car = car;
                    SetPositionIconShown(id, false);
                    // Vehicle icons are radar-only until made visible on the map.
                    Execute(fmt::format("game.navigation:SetIconMapVisibilityByNavId({}, true)", entry.vehicleNavId));
                }
            }
        }
    }

    void OnCarRemoved(SDK::C_Car *car) {
        for (auto &[id, entry] : gEntries) {
            if (entry.car == car) {
                UnregisterVehicleIcon(id, entry);
            }
        }
    }
} // namespace MafiaMP::Game::Helpers::Blips
