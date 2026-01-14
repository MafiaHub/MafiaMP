#pragma once

#include "../ue/c_ptr.h"

#include <cstdint>

namespace SDK {
    namespace prefab {
        // Forward declarations
        class I_PrefabWorker;
        struct S_GlobalInitData;

        enum E_PREFAB_TYPE {
            E_PT_Base             = 0,   // Base prefab type
            // 1 = unused/reserved
            E_PT_Car              = 2,   // Vehicles (cars)
            E_PT_CrashObj         = 3,   // Destructible/crash objects
            E_PT_ActorDeform      = 4,   // Deformable actors
            E_PT_Wheel            = 5,   // Vehicle wheels
            // 6 = unused/reserved
            E_PT_Door             = 7,   // Doors
            E_PT_Obsolete_Lift    = 8,   // (Obsolete) Lifts
            E_PT_Boat             = 9,   // Boats
            E_PT_Obsolete_Wagon   = 10,  // (Obsolete) Wagons
            E_PT_AIBrain          = 11,  // AI brain configurations
            E_PT_MountedWeapon    = 12,  // Mounted weapons
            E_PT_TrafficSemaphore = 13,  // Traffic lights
            E_PT_None             = 14,  // Default/no specific type
        };

        /**
         * Global init data structure used for prefab initialization.
         * Used with ue::C_Ptr for reference counting.
         */
        struct S_GlobalInitData {
            // Exact structure unknown, used as opaque pointer
        };

        /**
         * Interface for prefab workers that can process prefabs.
         */
        class I_PrefabWorker {
          public:
            virtual ~I_PrefabWorker() = default;
        };

        /**
         * Prefab manager singleton class that handles prefab lifecycle management.
         * Manages creation, storage, retrieval, and cleanup of prefabs.
         *
         * Class size: 184 bytes (0xB8)
         */
        class C_PrefabManager {
          public:
            /**
             * Get the singleton instance of the prefab manager.
             * Creates the instance if it doesn't exist.
             * @return Pointer to the singleton C_PrefabManager instance
             */
            static C_PrefabManager *GetInstance();

            /**
             * Get a prefab by GUID and type.
             * If the prefab doesn't exist and type is AI (11), creates it on the heap.
             * @param guid Reference to the prefab GUID
             * @param type The prefab type
             * @return Pointer to the prefab's S_GlobalInitData, or nullptr if not found
             */
            S_GlobalInitData *GetPrefab(const uint64_t &guid, E_PREFAB_TYPE type);

            /**
             * Get a prefab by GUID and type (overload with raw type parameter).
             * @param guid Reference to the prefab GUID
             * @param type The prefab type as unsigned int
             * @return Pointer to the prefab's S_GlobalInitData, or nullptr if not found
             */
            S_GlobalInitData *GetPrefab(const uint64_t &guid, unsigned int type);

            /**
             * Insert a prefab with the given GUID, type, and init data.
             * @param guid Reference to the prefab GUID
             * @param type The prefab type
             * @param initData Smart pointer to the init data
             */
            void InsertPrefab(const uint64_t &guid, E_PREFAB_TYPE type, ue::C_Ptr<S_GlobalInitData> initData);

            /**
             * Remove a specific prefab by GUID, type, and init data pointer.
             * @param guid Reference to the prefab GUID
             * @param type The prefab type
             * @param initData Pointer to the init data to remove
             */
            void RemovePrefab(const uint64_t &guid, E_PREFAB_TYPE type, S_GlobalInitData *initData);

            /**
             * Remove all prefabs of a specific type.
             * @param type The prefab type to remove
             */
            void RemovePrefabs(E_PREFAB_TYPE type);

            /**
             * Create a prefab on the heap for the given GUID and type.
             * Used internally when GetPrefab doesn't find an existing prefab for AI type.
             * @param guid Reference to the prefab GUID
             * @param type The prefab type
             * @return Smart pointer to the created prefab data
             */
            ue::C_Ptr<S_GlobalInitData> MakePrefabOnHeap(const uint64_t &guid, E_PREFAB_TYPE type);

            /**
             * Release all prefabs managed by this manager.
             */
            void ReleaseAll();

            /**
             * Set the prefab worker for this manager.
             * @param worker Pointer to the prefab worker
             */
            void SetPrefabWorker(I_PrefabWorker *worker);

            /**
             * Shutdown the prefab manager and release all resources.
             * Also destroys the singleton instance.
             */
            void Shutdown();

          private:
            C_PrefabManager();
            ~C_PrefabManager();

            // Prevent copying
            C_PrefabManager(const C_PrefabManager &)            = delete;
            C_PrefabManager &operator=(const C_PrefabManager &) = delete;

            // Static singleton instance
            static C_PrefabManager *m_PrefabManagerSingleton;

            // Class members (total size: 184 bytes / 0xB8)
            I_PrefabWorker *m_pPrefabWorker;  // 0x00 - 0x08
            void *m_pPrefabsStart;            // 0x08 - 0x10 (prefab container start)
            void *m_pPrefabsEnd;              // 0x10 - 0x18 (prefab container end)
            void *m_pPrefabsCapacity;         // 0x18 - 0x20 (prefab container capacity)
            char m_aUnkPadding1[8];           // 0x20 - 0x28 (not initialized in ctor)
            char m_aUnkStruct[24];            // 0x28 - 0x40 (cleaned by sub_1417C0270)
            void *m_pResourceListener;        // 0x40 - 0x48
            char m_aUnkStruct2[8];            // 0x48 - 0x50 (initialized by sub_14173CBA0)
            char m_aUnkStruct3[16];           // 0x50 - 0x60 (cleaned by sub_1417C17A0)
            void *m_pOwnedPrefabsStart;       // 0x60 - 0x68
            void *m_pOwnedPrefabsEnd;         // 0x68 - 0x70
            void *m_pOwnedPrefabsCapacity;    // 0x70 - 0x78
            char m_aUnkPadding2[16];          // 0x78 - 0x88
            char m_aMutex[48];                // 0x88 - 0xB8
        };

        static_assert(sizeof(C_PrefabManager) == 184, "Wrong size: prefab::C_PrefabManager");
    } // namespace prefab
} // namespace SDK
