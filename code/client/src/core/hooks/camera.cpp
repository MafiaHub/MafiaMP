#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

typedef int64_t(__fastcall *C_Camera__ModeChangeImmediate_t)(void *_this, int, void *, bool, bool, bool);
C_Camera__ModeChangeImmediate_t C_Camera__ModeChangeImmediate_original = nullptr;

typedef int64_t(__fastcall *C_Camera__SendCommand_t)(void *, unsigned int, void *, void *);
C_Camera__SendCommand_t C_Camera__SendCommand_original = nullptr;

typedef int64_t(__fastcall *C_GameCamera__CreateCamera_t)(void *, int64_t, int, void *);
C_GameCamera__CreateCamera_t C_GameCamera__CreateCamera_original = nullptr;

int64_t C_Camera__ModeChangeImmediate(void* _this, int mode, void* data, bool unk1, bool unk2, bool unk3) {
    Framework::Logging::GetLogger("Hooks")->debug("[Camera::ModeChangeImmediate]: id {}, ptr {:p}, {} {} {}", mode, fmt::ptr(data), unk1, unk2, unk3);
    return C_Camera__ModeChangeImmediate_original(_this, mode, data, unk1, unk2, unk3);
}

int64_t C_Camera__SendCommand(void* _this, unsigned int mode, void*unk1, void*unk2) {
    // Framework::Logging::GetLogger("Hooks")->debug("[Camera::SendCommand]: id {}, unk1 {:p}, unk2 {:p}", mode, fmt::ptr(unk1), fmt::ptr(unk2));
    return C_Camera__SendCommand_original(_this, mode, unk1, unk2);
}

int64_t C_GameCamera__CreateCamera(void *_this, int64_t a2, int a3, void *a4) {
    Framework::Logging::GetLogger("Hooks")->debug("[GameCamera::CreateCamera]: ID {:x}, Init Type {}, Camera Ptr {:p}", a2, a3, fmt::ptr(a4));
    return C_GameCamera__CreateCamera_original(_this, a2, a3, a4);
}

static InitFunction init([]() {
    const auto modeChangePattern = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC 30 0F B6 44 24 ? 48 8B D9").get_first());
    MH_CreateHook((LPVOID)modeChangePattern, (PBYTE)C_Camera__ModeChangeImmediate, reinterpret_cast<void **>(&C_Camera__ModeChangeImmediate_original));

    const auto sendCommandPattern = reinterpret_cast<uint64_t>(hook::pattern("48 8B 81 ? ? ? ? 48 8B 48 F8 48 8B 01 48 FF A0 D8 01 00 00").get_first());
    MH_CreateHook((LPVOID)sendCommandPattern, (PBYTE)C_Camera__SendCommand, reinterpret_cast<void **>(&C_Camera__SendCommand_original));

    const auto createCameraPattern = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 49 8B D9 41 8B F8"));
        MH_CreateHook((LPVOID)createCameraPattern, (PBYTE)C_GameCamera__CreateCamera, reinterpret_cast<void **>(&C_GameCamera__CreateCamera_original));
    }, "Camera");
