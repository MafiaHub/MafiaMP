#include "s_human_ai_request_play_anim_named.h"

#include "sdk/c_ie.h"
#include "sdk/patterns.h"
#include "sdk/ue/c_cnt_ptr.h"
#include "sdk/ue/game/script/c_sync_object.h"
#include "sdk/ue/sys/core/c_scene_object.h"
#include "sdk/ue/sys/math/c_matrix.h"

namespace SDK {
    namespace game::ai {
        S_HumanAIRequestPlayAnimNamed *S_HumanAIRequestPlayAnimNamed::Create(char const *animationName, bool loop) {
            auto *request = static_cast<S_HumanAIRequestPlayAnimNamed *>(C_IE::Alloc(sizeof(S_HumanAIRequestPlayAnimNamed)));
            if (!request) {
                return nullptr;
            }

            // What the AI command passes when the script gives no docking: an identity transform, no
            // object, no sync object, and an empty string for the second name, which the constructor
            // leaves unterminated when it is null.
            ue::sys::math::C_Matrix dockTransform;
            dockTransform.Identity();
            ue::C_WeakPtr<ue::sys::core::C_SceneObject> dockObject {};
            ue::C_CntPtr<ue::game::script::C_SyncObject> syncObject {};
            hook::this_call<void>(gPatterns.S_HumanAIRequestPlayAnimNamed__S_HumanAIRequestPlayAnimNamed, request, animationName, false, &dockTransform, false, dockObject, "", false, loop, false, &syncObject, 0.0f, 0.0f, 0.0f, false);
            request->AddRef();
            return request;
        }
    } // namespace game::ai
} // namespace SDK
