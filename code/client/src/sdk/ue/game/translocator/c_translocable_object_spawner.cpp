#include "c_translocable_object_spawner.h"

#include "../patterns.h"

namespace SDK {
    namespace ue::game::translocator {
        C_TranslocableObjectSpawner::C_TranslocableObjectSpawner() {
            // TODO: move pattern to patterns.h
            const auto pattern = hook::get_pattern("48 89 5C 24 ? 57 48 83 EC 30 33 FF 48 8D 05 ? ? ? ? 48 89 01 48 8B D9");
            hook::this_call<void>(pattern, this);
        }

        void* C_TranslocableObjectSpawner::operator new(size_t size) {
            return C_IE::Alloc(size);
        }

        void C_TranslocableObjectSpawner::operator delete(void* ptr) {
            C_IE::Free(ptr);
        }
    }
}
