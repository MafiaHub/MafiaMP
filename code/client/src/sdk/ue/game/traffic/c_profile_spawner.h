#pragma once

#include "../../../entities/c_actor.h"
#include "../../c_string.h"
#include "../../c_weak_ptr.h"
#include "../../sys/core/c_scene_object.h"
#include "../../sys/math/c_matrix.h"
#include "../../sys/math/c_quat.h"
#include "../../sys/math/c_vector.h"
#include "../../sys/utils/c_hash_name.h"

namespace SDK {
    namespace ue::game::traffic {
        class C_ProfileSpawner {
          protected:
            uintptr_t vftable;
            uint8_t pad[200] = {0};

          public:
            C_ProfileSpawner(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, const ue::sys::utils::C_HashName &, int, unsigned long unk2 = 10);
            C_ProfileSpawner(const C_ProfileSpawner &) = delete;
            ~C_ProfileSpawner();
            C_ProfileSpawner &operator=(const C_ProfileSpawner &) = delete;

            bool IsSpawnProfileLoaded();
            void CreateActor(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, C_Actor *&, void *unk1 = nullptr, unsigned int unk2 = 0, unsigned int unk3 = 0);
            void ReturnObject(ue::sys::core::C_SceneObject *);

            void *operator new(size_t);
            void operator delete(void *);
        };
    } // namespace ue::game::traffic
    static_assert(sizeof(ue::game::traffic::C_ProfileSpawner) == 208, "Wrong size: ue::game::traffic::C_ProfileSpawner");
} // namespace SDK