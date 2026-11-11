#pragma once

#include "shared/outfits.h"

#include <mafianet/types.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace MafiaMP::Core {
    // Outfits scripts made: spawn profiles defined by the pieces they wear. Clients build the
    // profile from its definition, which every client gets before it can see a ped wearing it.
    class Outfits final {
      public:
        // Returns the outfit's spawn profile hash, defining it for the clients the first time.
        uint64_t Define(const std::vector<Shared::Outfits::Piece> &pieces, uint64_t templateProfile);

        // Sends a joining player every outfit defined so far.
        void SendAll(MafiaNet::RakNetGUID guid) const;

      private:
        struct Definition {
            uint64_t templateProfile = 0;
            std::vector<Shared::Outfits::Piece> pieces;
        };

        void Send(uint64_t profile, const Definition &definition, MafiaNet::RakNetGUID guid, bool broadcast) const;

        std::unordered_map<uint64_t, Definition> _outfits;
    };
} // namespace MafiaMP::Core
