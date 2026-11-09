#include "s_human_ai_request_play_animation.h"

#include "../../../c_ie.h"
#include "../../../patterns.h"

#include <intrin.h>

namespace SDK {
    namespace ue::game::ai {
        void S_HumanAIRequestWeakProxy::Release() {
            if (_InterlockedDecrement(reinterpret_cast<volatile long *>(&m_RefCount)) == 0) {
                // Scalar deleting destructor.
                (*reinterpret_cast<void(__fastcall **)(S_HumanAIRequestWeakProxy *, uint64_t)>(m_pVtable))(this, 1);
            }
        }

        S_HumanAIRequestPlayAnimation *S_HumanAIRequestPlayAnimation::Create(char const *animationName, bool loop) {
            auto *request = static_cast<S_HumanAIRequestPlayAnimation *>(C_IE::Alloc(sizeof(S_HumanAIRequestPlayAnimation)));
            if (!request) {
                return nullptr;
            }

            // No docking transform, sync data or sync object: a plain animation, as the AI action
            // builds it from a PlayAnimation script call.
            uint8_t dockTransform[0x30] = {};
            uintptr_t syncData[3]       = {};
            uintptr_t syncObject        = 0;
            using Constructor_t         = S_HumanAIRequestPlayAnimation *(__fastcall *)(S_HumanAIRequestPlayAnimation *, char const *, bool, void *, bool, void *, char const *, bool, bool, bool, uintptr_t *, uint32_t, uint32_t, uint32_t, bool);
            reinterpret_cast<Constructor_t>(gPatterns.S_HumanAIRequestPlayAnimation__Constructor)(request, animationName, false, dockTransform, false, syncData, nullptr, false, loop, false, &syncObject, 0, 0, 0, false);
            ++request->m_RefCount;
            return request;
        }

        S_HumanAIRequestWeakProxy *S_HumanAIRequestPlayAnimation::CreateWeakProxy() {
            // The controller's request list conversion creates (or reuses) the proxy and adds a
            // reference for the caller; hand it a list of one. Its output is the controller's fixed
            // array of kMaxRequests slots: it clears every slot past the converted ones and releases
            // whatever they held, so the whole array must exist and start empty.
            struct {
                uintptr_t unk;
                int32_t count;
                char _pad[0x4];
                S_HumanAIRequestPlayAnimation *requests[1];
            } list {0, 1, {}, {this}};
            constexpr int kMaxRequests                       = 8;
            S_HumanAIRequestWeakProxy *proxies[kMaxRequests] = {};
            int32_t count                                    = 0;
            hook::call<void>(gPatterns.S_HumanAIRequestList__CollectWeakProxies, &list, &count, proxies);
            return proxies[0];
        }

        void S_HumanAIRequestPlayAnimation::Release() {
            if (--m_RefCount == 0) {
                auto *counted = reinterpret_cast<uintptr_t *>(&m_pRefCountVtable);
                (*reinterpret_cast<void(__fastcall **)(uintptr_t *)>(*counted + 0x8))(counted);
            }
        }
    } // namespace ue::game::ai
} // namespace SDK
