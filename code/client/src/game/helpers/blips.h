#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <string>

namespace SDK {
    class C_Car;
}

// Map blips, keyed by the server's blip id. A blip is a position icon drawn through the game's
// navigation script API. One with a vehicle icon that follows a streamed-in vehicle is drawn as
// that vehicle's own navigation icon instead, until the vehicle streams out.
namespace MafiaMP::Game::Helpers::Blips {
    void Set(uint32_t id, const std::string &icon, const glm::vec3 &position, uint64_t attachedTo);
    void Remove(uint32_t id);
    void Clear();

    // Shows the GPS route to a blip; 0 clears it.
    void SetRoute(uint32_t id);

    // Swaps between vehicle and position icons as followed vehicles stream in and out.
    void Update();

    // Drops the vehicle icons of a car before the game destroys it.
    void OnCarRemoved(SDK::C_Car *car);
} // namespace MafiaMP::Game::Helpers::Blips
