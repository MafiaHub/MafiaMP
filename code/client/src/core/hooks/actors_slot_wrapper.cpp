#include <MinHook.h>
#include <utils/hooking/hook_function.h>
#include <utils/hooking/hooking.h>

#include <logging/logger.h>

#include "sdk/mafia/streaming/c_actors_slot_wrapper.h"
#include "sdk/mafia/streaming/c_slot.h"
#include "sdk/ue/c_string.h"
#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/patterns.h"

namespace {
    // Case-insensitive substring match
    bool ModelNameMatches(const char *sceneObjectName, const char *targetModel) {
        if (!sceneObjectName || !targetModel) {
            return false;
        }

        std::string objNameLower(sceneObjectName);
        std::string targetLower(targetModel);

        for (auto &c : objNameLower) c = static_cast<char>(tolower(c));
        for (auto &c : targetLower) c = static_cast<char>(tolower(c));

        return objNameLower.find(targetLower) != std::string::npos;
    }
} // namespace

// Hook for the StartCaching visitor callback (sub_1422C77D0)
// Parameters:
//   a1 = lambda context: *(a1+0) = pThis, *(a1+8) = output pointer
//   a2 = pointer to scene object pointer
typedef void(__fastcall *StartCachingVisitor_t)(uintptr_t a1, uintptr_t *a2);
StartCachingVisitor_t StartCachingVisitor_original = nullptr;

void __fastcall StartCachingVisitor_Hook(uintptr_t a1, uintptr_t *a2) {
    auto logger = Framework::Logging::GetLogger("Hooks");

    // Get the wrapper from the lambda context
    auto *pThis = *reinterpret_cast<SDK::mafia::streaming::C_ActorsSlotWrapper **>(a1);
    if (!pThis) {
        StartCachingVisitor_original(a1, a2);
        return;
    }

    // Get target model name from the wrapper
    const char *targetModel = pThis->GetModelName();
    if (!targetModel || !*targetModel) {
        // No specific model requested, use default behavior
        StartCachingVisitor_original(a1, a2);
        return;
    }

    // Get the current scene object being processed
    uintptr_t sceneObjPtr = *a2;
    if (!sceneObjPtr) {
        StartCachingVisitor_original(a1, a2);
        return;
    }

    auto *sceneObj = reinterpret_cast<SDK::ue::sys::core::C_SceneObject *>(sceneObjPtr);
    const char *sceneObjName = sceneObj->GetName();
    if (!sceneObjName) {
        StartCachingVisitor_original(a1, a2);
        return;
    }

    // Check if this scene object matches our target model
    if (ModelNameMatches(sceneObjName, targetModel)) {
        logger->debug("[ActorsSlotWrapper] Visitor: Found matching scene object '{}' for target '{}'",
                      sceneObjName, targetModel);

        // Force-store this scene object in the output, bypassing priority logic
        // Output pointer is at *(a1 + 8), and we write the scene object ptr there
        uintptr_t *outputPtr = *reinterpret_cast<uintptr_t **>(a1 + 8);
        if (outputPtr) {
            *outputPtr = sceneObjPtr;
            logger->debug("[ActorsSlotWrapper] Visitor: Forced selection of '{}'", sceneObjName);
        }
        return;
    }

    // IMPORTANT: When we have a specific target model, do NOT call the original function
    // for non-matching objects. The original function uses priority-based selection which
    // would overwrite our forced selection with higher-priority but wrong objects.
    // Simply skip non-matching objects entirely.
}

static InitFunction init(
    []() {
        MH_CreateHook(
            (LPVOID)SDK::gPatterns.C_ActorsSlotWrapper__StartCachingVisitor,
            (PBYTE)StartCachingVisitor_Hook,
            reinterpret_cast<void **>(&StartCachingVisitor_original));
    },
    "ActorsSlotWrapper");
