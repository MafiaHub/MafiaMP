#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace MafiaMP::Shared::Outfits {
    // A combinable piece a ped wears, and the package the game loads it from.
    struct Piece {
        uint64_t pieceId   = 0;
        uint64_t packageId = 0;
    };

    // The most pieces an outfit holds; real profiles wear about a dozen.
    inline constexpr size_t kMaxPieces = 64;

    // The spawn profile hash of an outfit: FNV-1a over its sorted piece ids, so the server and every
    // client name the same outfit alike whatever the order the pieces came in.
    inline uint64_t HashOf(const std::vector<Piece> &pieces) {
        std::vector<uint64_t> ids;
        ids.reserve(pieces.size());
        for (const auto &piece : pieces) {
            ids.push_back(piece.pieceId);
        }
        std::sort(ids.begin(), ids.end());
        uint64_t hash = 0xcbf29ce484222325ULL;
        for (const uint64_t id : ids) {
            for (int i = 0; i < 8; ++i) {
                hash = (hash ^ ((id >> (i * 8)) & 0xFF)) * 0x100000001b3ULL;
            }
        }
        return hash;
    }
} // namespace MafiaMP::Shared::Outfits
