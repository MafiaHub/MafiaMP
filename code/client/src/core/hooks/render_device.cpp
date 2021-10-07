#include <MinHook.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hook_function.h>

#include <logging/logger.h>

#include <d3d11.h>

#include "../../sdk/ue/sys/render/device/c_direct_3d11_render_device.h"
#include "../../sdk/ue/sys/render/device/s_render_device_desc.h"
#include "../../sdk/ue/sys/render/device/c_dynamic_vi_buffer_pool.h"


typedef bool(__fastcall *C_RenderDevice_Unknown_t)(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *pThis, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2, SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk);
C_RenderDevice_Unknown_t C_RenderDevice_Unknown_original = nullptr;

bool C_RenderDevice_Unknown(SDK::ue::sys::render::device::C_Direct3D11RenderDevice *device, SDK::ue::sys::render::device::S_RenderDeviceDesc const &a2,
    SDK::ue::sys::render::device::C_DynamicVIBufferPool &a3, void *idk) {

    //TODO: initialize rendering backend + gui library
    return C_RenderDevice_Unknown_original(device, a2, a3, idk);
}

static InitFunction init([]() {
    const auto C_Unknown_Addr = hook::get_opcode_address("E8 ? ? ? ? 0F 10 03 33 ED");
    MH_CreateHook((LPVOID)C_Unknown_Addr, (PBYTE)C_RenderDevice_Unknown, reinterpret_cast<void **>(&C_RenderDevice_Unknown_original));
});
