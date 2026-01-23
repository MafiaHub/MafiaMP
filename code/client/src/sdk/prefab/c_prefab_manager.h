#pragma once

#include "../ue/c_ptr.h"

#include <cstdint>

namespace SDK {
    namespace prefab {
        // Forward declarations
        class I_PrefabWorker;
        struct S_GlobalInitData;

        enum E_PREFAB_TYPE {
            E_PT_Base             = 0,
            // 1 = unused/reserved
            E_PT_Car              = 2,
            E_PT_CrashObj         = 3,
            E_PT_ActorDeform      = 4,
            E_PT_Wheel            = 5,
            // 6 = unused/reserved
            E_PT_Door             = 7,
            E_PT_Obsolete_Lift    = 8,
            E_PT_Boat             = 9,
            E_PT_Obsolete_Wagon   = 10,
            E_PT_AIBrain          = 11,
            E_PT_MountedWeapon    = 12,
            E_PT_TrafficSemaphore = 13,
            E_PT_None             = 14,
        };

        // TODO: Reverse complete structure
        struct S_GlobalInitData {};

        class I_PrefabWorker {
          public:
            virtual ~I_PrefabWorker() = default;
        };

        // Singleton prefab manager - 184 bytes
        class C_PrefabManager {
          public:
            static C_PrefabManager *GetInstance();

            S_GlobalInitData *GetPrefab(const uint64_t &guid, E_PREFAB_TYPE type);
            S_GlobalInitData *GetPrefab(const uint64_t &guid, unsigned int type);
            void InsertPrefab(const uint64_t &guid, E_PREFAB_TYPE type, ue::C_Ptr<S_GlobalInitData> initData);
            void RemovePrefab(const uint64_t &guid, E_PREFAB_TYPE type, S_GlobalInitData *initData);
            void RemovePrefabs(E_PREFAB_TYPE type);
            ue::C_Ptr<S_GlobalInitData> MakePrefabOnHeap(const uint64_t &guid, E_PREFAB_TYPE type);
            void ReleaseAll();
            void SetPrefabWorker(I_PrefabWorker *worker);
            void Shutdown();

          private:
            C_PrefabManager();
            ~C_PrefabManager();

            C_PrefabManager(const C_PrefabManager &)            = delete;
            C_PrefabManager &operator=(const C_PrefabManager &) = delete;

            static C_PrefabManager *m_PrefabManagerSingleton;

            I_PrefabWorker *m_pPrefabWorker;  // 0x00 - 0x08
            void *m_pPrefabsStart;            // 0x08 - 0x10
            void *m_pPrefabsEnd;              // 0x10 - 0x18
            void *m_pPrefabsCapacity;         // 0x18 - 0x20
            char m_aUnkPadding1[8];           // 0x20 - 0x28
            char m_aUnkStruct[24];            // 0x28 - 0x40
            void *m_pResourceListener;        // 0x40 - 0x48
            char m_aUnkStruct2[8];            // 0x48 - 0x50
            char m_aUnkStruct3[16];           // 0x50 - 0x60
            void *m_pOwnedPrefabsStart;       // 0x60 - 0x68
            void *m_pOwnedPrefabsEnd;         // 0x68 - 0x70
            void *m_pOwnedPrefabsCapacity;    // 0x70 - 0x78
            char m_aUnkPadding2[16];          // 0x78 - 0x88
            char m_aMutex[48];                // 0x88 - 0xB8
        };

        static_assert(sizeof(C_PrefabManager) == 184, "Wrong size: prefab::C_PrefabManager");
    } // namespace prefab
} // namespace SDK
