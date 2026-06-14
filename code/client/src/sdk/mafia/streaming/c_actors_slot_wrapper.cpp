#include "c_actors_slot_wrapper.h"

#include "../../c_ie.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::streaming {
        C_ActorsSlotWrapper::C_ActorsSlotWrapper(E_EntityType type, unsigned int maxActorsCnt, unsigned int maxCrewCnt, char const *a5, bool allowDuplicationCaching) {
            hook::this_call<void>(gPatterns.C_ActorsSlotWrapper__C_ActorsSlotWrapper, this, type, maxActorsCnt, maxCrewCnt, a5, allowDuplicationCaching);
        }

        void C_ActorsSlotWrapper::UpdateToCreateActors(unsigned int a2, int a3) {
            hook::this_call<void>(gPatterns.C_ActorsSlotWrapper__UpdateToCreateActors, this, a2, a3);
        }

        void C_ActorsSlotWrapper::GetFreeActor(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &a2, bool a3, C_Actor **a4) {
            (*(void(__thiscall *)(C_ActorsSlotWrapper *, ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, bool, C_Actor **))gPatterns.C_ActorsSlotWrapper__GetFreeActor)(this, a2, a3, a4);
        }

        void C_ActorsSlotWrapper::ReturnActor(C_Actor *pActor) {
            hook::this_call<void>(gPatterns.C_ActorsSlotWrapper__ReturnActor, this, pActor);
        }

        void C_ActorsSlotWrapper::Close(bool destroyActorItems) {
            hook::this_call<void>(gPatterns.C_ActorsSlotWrapper__Close, this, destroyActorItems);
        }

        bool C_ActorsSlotWrapper::StartCaching() {
            return hook::this_call<bool>(gPatterns.C_ActorsSlotWrapper__StartCaching, this);
        }

        void *C_ActorsSlotWrapper::operator new(size_t size) {
            return C_IE::Alloc(size);
        }

        void C_ActorsSlotWrapper::operator delete(void *ptr) {
            C_IE::Free(ptr);
        }
    } // namespace mafia::streaming
} // namespace SDK
