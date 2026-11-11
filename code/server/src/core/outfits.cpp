#include "outfits.h"

#include "shared/rpc/ids.h"

#include <core_modules.h>
#include <networking/network_peer.h>

#include <mafianet/bit_stream.h>

namespace MafiaMP::Core {
    uint64_t Outfits::Define(const std::vector<Shared::Outfits::Piece> &pieces, uint64_t templateProfile) {
        if (pieces.empty() || pieces.size() > Shared::Outfits::kMaxPieces) {
            return 0;
        }
        const uint64_t profile = Shared::Outfits::HashOf(pieces);
        if (_outfits.contains(profile)) {
            return profile;
        }
        const auto &definition = _outfits[profile] = {templateProfile, pieces};
        Send(profile, definition, MafiaNet::UNASSIGNED_RAKNET_GUID, true);
        return profile;
    }

    void Outfits::SendAll(MafiaNet::RakNetGUID guid) const {
        for (const auto &[profile, definition] : _outfits) {
            Send(profile, definition, guid, false);
        }
    }

    void Outfits::Send(uint64_t profile, const Definition &definition, MafiaNet::RakNetGUID guid, bool broadcast) const {
        auto *net = Framework::CoreModules::GetNetworkPeer();
        if (!net) {
            return;
        }
        // Wire: <profile><template profile><piece count u16>(<piece id><package id>)*
        MafiaNet::BitStream bs;
        bs.Write(profile);
        bs.Write(definition.templateProfile);
        bs.Write(static_cast<uint16_t>(definition.pieces.size()));
        for (const auto &piece : definition.pieces) {
            bs.Write(piece.pieceId);
            bs.Write(piece.packageId);
        }
        net->GetRPC()->Signal(net->ResolveIdentifier(Shared::RPC::kDefineOutfit), &bs, MafiaNet::Priority::High, MafiaNet::Reliability::ReliableOrdered, 0, guid, broadcast, false);
    }
} // namespace MafiaMP::Core
