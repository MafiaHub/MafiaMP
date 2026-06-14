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
            E_EntityType m_ActorType;              // 0x18 - 0x1C
            unsigned int m_CurrentMaxActorsCnt;    // 0x1C - 0x20
            unsigned int m_MaxActorsCnt;           // 0x20 - 0x24
            unsigned int m_CurrentMaxCrewCnt;      // 0x24 - 0x28
            unsigned int m_MaxCrewCnt;             // 0x28 - 0x2C
            uint8_t m_Unk44[36];                   // 0x2C - 0x50
            unsigned int m_FreeActorsCount;        // 0x50 - 0x54
            unsigned int m_UsedActorsCount;        // 0x54 - 0x58
            unsigned int m_FreeCrew;               // 0x58 - 0x5C
            unsigned int m_UsedCrew;               // 0x5C - 0x60
            bool m_AllowDuplicationCaching;        // 0x60 - 0x61
            char _pad61[3];                        // 0x61 - 0x64
            E_ActorsSlotStates m_ActorsState;      // 0x64 - 0x68
            uint8_t m_Unk104[72];                  // 0x68 - 0xB0
            uint64_t m_SceneObjectCacheKey;        // 0xB0 - 0xB8 (used by TryGetObject)
            char *m_CurrentFileName;               // 0xB8 - 0xC0 (raw string, not C_String*)
            unsigned int m_UsedForDemanded;        // 0xC0 - 0xC4
            uint8_t m_Unk196[12];                  // 0xC4 - 0xD0

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

            uint64_t GetSceneObjectCacheKey() const {
                return m_SceneObjectCacheKey;
            }

            void SetSceneObjectCacheKey(uint64_t key) {
                m_SceneObjectCacheKey = key;
            }

            C_Slot *GetSlot() const {
                return m_pSlot;
            }

            const char *GetModelName() const {
                return m_CurrentFileName;
            }

            void UpdateToCreateActors(unsigned int, int);
            void GetFreeActor(ue::C_WeakPtr<ue::sys::core::C_SceneObject> &, bool, C_Actor **);
            void ReturnActor(C_Actor *);
            void Close(bool destroyActorItems);
            bool StartCaching();

            void *operator new(size_t);
            void operator delete(void *);
        };
    } // namespace mafia::streaming
    static_assert(sizeof(mafia::streaming::C_ActorsSlotWrapper) == 208, "Wrong size: mafia::streaming::C_ActorsSlotWrapper");
}
