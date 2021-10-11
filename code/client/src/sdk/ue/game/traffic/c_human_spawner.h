#pragma once

#include "c_profile_spawner.h"

namespace SDK {
    namespace ue::game::traffic {
        class C_HumanSpawner: public C_ProfileSpawner {
          public:
            C_HumanSpawner(const ue::sys::utils::C_HashName &, int);
            ~C_HumanSpawner();

            ue::C_WeakPtr<ue::sys::core::C_SceneObject> SetupDefaultArchetypeObject();

            void *operator new(size_t size) {
                return C_ProfileSpawner::operator new(size);
            }
            void operator delete(void *ptr) {
                C_ProfileSpawner::operator delete(ptr);
            }
        };
    } // namespace ue::game::traffic
    static_assert(sizeof(ue::game::traffic::C_HumanSpawner) == 208, "Wrong size: ue::game::traffic::C_HumanSpawner");
} // namespace SDK