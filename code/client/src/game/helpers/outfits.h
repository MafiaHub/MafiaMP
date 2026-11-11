#pragma once

#include "shared/outfits.h"

#include <cstdint>
#include <vector>

// Outfits: spawn profiles made at runtime from a list of combinable pieces. Each piece is worn as
// is, so a ped spawned with the outfit's profile looks the same on every client.
namespace MafiaMP::Game::Helpers::Outfits {
    using Piece = Shared::Outfits::Piece;

    // Adds the outfit's spawn profile to the game's database, built on the template profile (its
    // body ranges, flags and priority), and returns its hash. 0 if it could not be built.
    uint64_t Register(const std::vector<Piece> &pieces, uint64_t templateProfile);

    // Keeps an outfit the server defined, to register once a ped wears it.
    void Define(uint64_t profile, uint64_t templateProfile, std::vector<Piece> pieces);

    // Whether a ped can be spawned with the profile: one the game has, or an outfit defined by the
    // server, which is registered on the way. False for an outfit whose definition is still due.
    bool Ensure(uint64_t profile);
} // namespace MafiaMP::Game::Helpers::Outfits
