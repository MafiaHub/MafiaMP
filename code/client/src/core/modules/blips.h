#pragma once

#include "sdk/entities/c_car.h"

// Map blips, keyed by the server's blip id. A blip is a position icon on the map and the radar. One
// that follows an entity streamed in here follows the game's own actor; elsewhere it stands where the
// server last put it. One with a vehicle icon that follows a streamed-in car is drawn as that car's
// own navigation icon instead, while the car is here.
namespace MafiaMP::Core::Modules::Blips {
    void Install();

    // Draws what the game could not take yet and moves the blips that follow a streamed-in entity.
    void UpdateAll();

    // Drops every blip, for a session that ended.
    void Clear();

    // Drops the vehicle icon of a car before the game destroys it.
    void OnCarRemoved(SDK::C_Car *car);
} // namespace MafiaMP::Core::Modules::Blips
