#include "c_stream_map.h"

#include <utils/hooking/hooking.h>
#include "patterns.h"

namespace SDK {
    void C_StreamMap::CloseGame() {
        hook::this_call(gPatterns.C_StreamMap__CloseGame, this);
    }

    void C_StreamMap::CloseMission() {
        hook::this_call(gPatterns.C_StreamMap__CloseMission, this);
    }

    void C_StreamMap::ClosePart() {
        hook::this_call(gPatterns.C_StreamMap__ClosePart, this);
    }

    const char* C_StreamMap::GetGame() {
        return hook::this_call<const char *>(gPatterns.C_StreamMap__GetGame, this);
    }

    const char* C_StreamMap::GetMission() {
        return hook::this_call<const char *>(gPatterns.C_StreamMap__GetMission, this);
    }

    const char* C_StreamMap::GetPart() {
        return hook::this_call<const char *>(gPatterns.C_StreamMap__GetPart, this);
    }

    void C_StreamMap::OpenGame(const char* game) {
        hook::this_call(gPatterns.C_StreamMap__OpenGame, this, game);
    }

    void C_StreamMap::OpenMission(const char* mission) {
        hook::this_call(gPatterns.C_StreamMap__OpenMission, this, mission);
    }

    void C_StreamMap::OpenPart(const char* part) {
        hook::this_call(gPatterns.C_StreamMap__OpenPart, this, part);
    }
}
