#pragma once

#include <cstdint>

#include "../../entities/c_entity.h"
#include "../../ue/c_string.h"
#include "../../ue/c_weak_ptr.h"
#include "../../ue/sys/core/c_scene_object.h"
#include "../../c_time_budget_info.h"
#include "c_slot_wrapper.h"

namespace SDK {
    namespace mafia::streaming {
        class C_ActorsSlotWrapper: public C_SlotWrapper {
          public:
            enum E_ActorsSlotStates { E_ASS_WAITING_FOR_LOAD, E_ASS_CREATING, E_ASS_READY, E_ASS_RELEASING, E_ASS_RELEASED, E_ASS_DATA_DELETED };

          protected:
            E_EntityType m_ActorType;
            unsigned int m_CurrentMaxActorsCnt;
            unsigned int m_MaxActorsCnt;
            unsigned int m_CurrentMaxCrewCnt;
            unsigned int m_MaxCrewCnt;
            uint8_t m_Unk44[36];
            unsigned int m_FreeActorsCount;
            unsigned int m_UsedActorsCount;
            unsigned int m_FreeCrew;
            unsigned int m_UsedCrew;
            bool m_AllowDuplicationCaching;
            E_ActorsSlotStates m_ActorsState;
            uint8_t m_Unk104[80];
            ue::C_String *m_CurrentFileName;
            unsigned int m_UsedForDemanded;
            uint8_t m_Unk196[12];

          public:
            virtual void Update(C_TimeBudgetInfo::C_Ctx const &) {
            }
            virtual void DbgDraw() {
            }
            virtual const ue::C_String *GetCurrentFileName() {
                return nullptr;
            }
            virtual bool AllowDuplicationCaching() {
                return false;
            }
            virtual bool CreateActor(int) {
                return false;
            }

          public:
            C_ActorsSlotWrapper(E_EntityType, unsigned int maxActorsCnt, unsigned int maxCrewCnt, char const *, bool allowDuplicationCaching);

            E_EntityType GetActorType() const {
                return m_ActorType;
            }
            unsigned int GetToCreateActorsWithoutCrewCnt() const {
                return m_CurrentMaxActorsCnt;
            }
            unsigned int GetToCreateActorsCrewCnt() const {
                return m_CurrentMaxCrewCnt;
            }
            unsigned int GetNumFree() const {
                return (m_FreeActorsCount - m_FreeCrew);
            }
            unsigned int GetNumUsed() const {
                return (m_UsedActorsCount - m_UsedCrew);
            }
            unsigned int GetCreatedActorsCnt() const {
                return (m_FreeActorsCount + m_UsedActorsCount);
            }
            unsigned int GetNumFreeCrew() const {
                return m_FreeCrew;
            }
            unsigned int GetNumUsedCrew() const {
                return m_UsedCrew;
            }
            bool AllowDuplicationCaching() const {
                return m_AllowDuplicationCaching;
            }
            E_ActorsSlotStates GetActorsState() const {
                return m_ActorsState;
            }
            bool GetActorsReady() const {
                return (m_ActorsState == E_ASS_READY);
            }
            bool GetUsedForDemanded() const {
                return (m_UsedForDemanded > 0);
            }

            void UpdateToCreateActors(unsigned int, int);
            void GetFreeActor(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, bool, C_Actor **);
            void ReturnActor(C_Actor *);
            void Close(bool destroyActorItems);

            void *operator new(size_t);
            void operator delete(void *);
        };
    } // namespace mafia::streaming
    static_assert(sizeof(mafia::streaming::C_ActorsSlotWrapper) == 208, "Wrong size: mafia::streaming::C_ActorsSlotWrapper");
}
