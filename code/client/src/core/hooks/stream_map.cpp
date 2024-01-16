#include <MinHook.h>
#include <utils/hooking/hook_function.h>

#include "../../sdk/c_stream_map.h"
#include "../../sdk/patterns.h"

#include <logging/logger.h>

typedef void(__fastcall *C_StreamMap__OpenGame_t)(SDK::C_StreamMap *, const char *);
C_StreamMap__OpenGame_t C_StreamMap__OpenGame_original = nullptr;
void C_StreamMap__OpenGame(SDK::C_StreamMap *pThis, const char *game) {
    C_StreamMap__OpenGame_original(pThis, game);
    Framework::Logging::GetLogger("Hooks")->debug("C_StreamMap::OpenGame: Opened game {}", game);
}

typedef void(__fastcall *C_StreamMap__OpenMission_t)(SDK::C_StreamMap *, const char *);
C_StreamMap__OpenMission_t C_StreamMap__OpenMission_original = nullptr;
void C_StreamMap__OpenMission(SDK::C_StreamMap *pThis, const char *mission) {
    C_StreamMap__OpenMission_original(pThis, mission);
    Framework::Logging::GetLogger("Hooks")->debug("C_StreamMap::OpenMission: Opened mission {}", mission);
}

typedef void(__fastcall *C_StreamMap__OpenPart_t)(SDK::C_StreamMap *, const char *);
C_StreamMap__OpenPart_t C_StreamMap__OpenPart_original = nullptr;
void C_StreamMap__OpenPart(SDK::C_StreamMap *pThis, const char *part) {
    C_StreamMap__OpenPart_original(pThis, part);
    Framework::Logging::GetLogger("Hooks")->debug("C_StreamMap::OpenPart: Opened part {}", part);
}

typedef void(__fastcall *C_StreamMap__CloseGame_t)(SDK::C_StreamMap *);
C_StreamMap__CloseGame_t C_StreamMap__CloseGame_original = nullptr;
void C_StreamMap__CloseGame(SDK::C_StreamMap *pThis) {
    const auto game = pThis->GetGame();
    C_StreamMap__CloseGame_original(pThis);
    Framework::Logging::GetLogger("Hooks")->debug("C_StreamMap::CloseGame: Closed game {}", game);
}

typedef void(__fastcall *C_StreamMap__CloseMission_t)(SDK::C_StreamMap *);
C_StreamMap__CloseMission_t C_StreamMap__CloseMission_original = nullptr;
void C_StreamMap__CloseMission(SDK::C_StreamMap *pThis) {
    const auto mission = pThis->GetMission();
    C_StreamMap__CloseMission_original(pThis);
    Framework::Logging::GetLogger("Hooks")->debug("C_StreamMap::CloseMission: Closed mission {}", mission);
}

typedef void(__fastcall *C_StreamMap__ClosePart_t)(SDK::C_StreamMap *);
C_StreamMap__ClosePart_t C_StreamMap__ClosePart_original = nullptr;
void C_StreamMap__ClosePart(SDK::C_StreamMap *pThis) {
    const auto part = pThis->GetPart();
    C_StreamMap__ClosePart_original(pThis);
    Framework::Logging::GetLogger("Hooks")->debug("C_StreamMap::ClosePart: Closed part {}", part);
}

static InitFunction init([]() {
    MH_CreateHook((LPVOID)SDK::gPatterns.C_StreamMap__OpenGame, (PBYTE)C_StreamMap__OpenGame, reinterpret_cast<void **>(&C_StreamMap__OpenGame_original));
    MH_CreateHook((LPVOID)SDK::gPatterns.C_StreamMap__OpenMission, (PBYTE)C_StreamMap__OpenMission, reinterpret_cast<void **>(&C_StreamMap__OpenMission_original));
    MH_CreateHook((LPVOID)SDK::gPatterns.C_StreamMap__OpenPart, (PBYTE)C_StreamMap__OpenPart, reinterpret_cast<void **>(&C_StreamMap__OpenPart_original));
    MH_CreateHook((LPVOID)SDK::gPatterns.C_StreamMap__CloseGame, (PBYTE)C_StreamMap__CloseGame, reinterpret_cast<void **>(&C_StreamMap__CloseGame_original));
    MH_CreateHook((LPVOID)SDK::gPatterns.C_StreamMap__CloseMission, (PBYTE)C_StreamMap__CloseMission, reinterpret_cast<void **>(&C_StreamMap__CloseMission_original));
    MH_CreateHook((LPVOID)SDK::gPatterns.C_StreamMap__ClosePart, (PBYTE)C_StreamMap__ClosePart, reinterpret_cast<void **>(&C_StreamMap__ClosePart_original));
});
