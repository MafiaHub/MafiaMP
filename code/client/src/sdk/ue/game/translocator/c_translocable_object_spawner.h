#pragma once

#include "../c_ie.h"

namespace SDK {
    namespace ue::game::translocator {
        class C_TranslocableObjectSpawner {
            C_TranslocableObjectSpawner::C_TranslocableObjectSpawner();

            void *operator new(size_t);
            void operator delete(void *);
        };
    }
}
