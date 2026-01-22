#pragma once

#include "../prefab/c_prefab_init_data.h"
#include "../prefab/c_prefab_manager.h"
#include "../ue/sys/math/c_vector.h"
#include "../ue/sys/core/c_scene_object.h"

#include <cstdint>

namespace SDK {
    enum class E_EntityType : uint8_t {
        E_ENTITY_UNKNOWN,
        E_ENTITY_ENTITY,
        E_ENTITY_ENTITY_POS,
        E_ENTITY_ENTITY_DUMMY,
        E_ENTITY_ACTOR,
        E_ENTITY_TICKED_MODULE,
        E_ENTITY_TICKED_MODULE_MANAGER,
        E_ENTITY_BASE_HUMAN,
        E_ENTITY_HUMAN,
        E_ENTITY_PLAYER,
        E_ENTITY_CAR,
        E_ENTITY_CRASH_OBJECT  = 12,
        E_ENTITY_GLASSBREAKING = 20,
        E_ENTITY_DOOR,
        E_ENTITY_TREE,
        E_ENTITY_SOUND = 24,
        E_ENTITY_GRENADE,
        E_ENTITY_MOLOTOV,
        E_ENTITY_BOAT,
        E_ENTITY_SLOT_MACHINE = 30,
        E_ENTITY_ANIMAL,
        E_ENTITY_STATIC_ENTITY,
        E_ENTITY_TRANSLOCATED_CAR,
        E_ENTITY_GARAGE,
        E_ENTITY_FRAME_WRAPPER,
        E_ENTITY_ACTOR_DETECTOR,
        E_ENTITY_FAKE_MODEL_TRANSLOCATOR,
        E_ENTITY_BLOCKER,
        E_ENTITY_STATIC_PARTICLE = 40,
        E_ENTITY_FIRETARGET,
        E_ENTITY_CUTSCENE = 43,
        E_ENTITY_DUMMY,
        E_ENTITY_TELEPHONE,
        E_ENTITY_SCRIPT,
        E_ENTITY_PATH_OBJECT,
        E_ENTITY_SCRIPTCMDBUFFER,
        E_ENTITY_SCRIPTDELAYBUFFER,
        E_ENTITY_ROADBLOCK,
        E_ENTITY_DAMAGE_ZONE,
        E_ENTITY_AIRCRAFT,
        E_ENTITY_TV,
        E_ENTITY_PINUP,
        E_ENTITY_SPIKE_STRIP,
        E_ENTITY_SCRIPT_SHOPMENU = 59,
        E_ENTITY_PHYSICS_SCENE,
        E_ENTITY_CLEAN_ENTITY,
        E_ENTITY_ITEM_WEAPON = 63,
        E_ENTITY_ITEM_SCRIPT_AB,
        E_ENTITY_NAVDUMMY,
        E_ENTITY_ITEM_INVENTORY,
        E_ENTITY_ITEM_UPGRADE = 68,
        E_ENTITY_TRAP,
        E_ENTITY_NAV_BOX,
        E_ENTITY_FAKE_ENEMY         = 73,
        E_ENTITY_PIER               = 75,
        E_ENTITY_TAIL               = 77,
        E_ENTITY_BATTLE_DESTINATION = 80,
        E_ENTITY_MOUNTED_WEAPON,
        E_ENTITY_TRAIL = 83,
        E_ENTITY_TRAFFIC_SEMAPHORE,
        E_ENTITY_HEAD_QUARTERS,
        E_ENTITY_CRIME_BUSINESS,
        E_ENTITY_BED,
        E_ENTITY_PROJECTILE_EXPLOSIVE,
        E_ENTITY_SATCHEL_CHARGE = 90,
        E_ENTITY_LAST_ID
    };

    class C_Entity;
    class I_EntityListener {
      public:
        virtual ~I_EntityListener() {}
        virtual void OnEvent(C_Entity *, /*E_EntityEvents*/ int) {}
    };

    enum class E_UpdateRate : uint8_t {
        E_UPDATE_RATE_UNK0 = 0,
        E_UPDATE_RATE_UNK1 = 1,
        E_UPDATE_RATE_UNK2 = 2,
        E_UPDATE_RATE_UNK5 = 5
    };

    class C_Actor;
    class C_Entity {
      public:
        virtual ~C_Entity()                                                    = 0;
        virtual uint64_t GetClassTypeInfo(void)                                = 0;
        virtual uint64_t GetTypeInfo(void)                                     = 0;
        virtual bool IsInCachingProcess(void)                                  = 0;
        virtual void OnInit(void)                                              = 0;
        virtual void OnGameInit(void)                                          = 0;
        virtual void OnGameDone(void)                                          = 0;
        virtual void OnGameRestore(void)                                       = 0;
        virtual void OnActivate(void)                                          = 0;
        virtual void OnDeactivate(void)                                        = 0;
        virtual void OnUpdate(float)                                           = 0;
        virtual void OnRecvMessage(/*C_EntityMessage**/ void *)                = 0;
        virtual void OnSceneObjectChanged(void *)                              = 0;
        virtual void SetProhibitGarage(bool)                                   = 0;
        virtual uint64_t GetProhibitGarage(void)                               = 0;
        virtual uint64_t GetAvailableAction(C_Actor *, void *, void *)         = 0;
        virtual uint64_t CreateSaveBlock(void)                                 = 0;
        virtual void OnGameSaveBody(void *)                                    = 0;
        virtual void OnGameLoadBody(void *)                                    = 0;
        virtual void GameLoadPrerequisite(void *)                              = 0;
        virtual void GameLoad(void *)                                          = 0;
        virtual void GameLoadDependencies(void *)                              = 0;
        virtual void AddOnEvent(unsigned int, unsigned int, unsigned int)      = 0;
        virtual bool IsSafeSpawn(ue::sys::math::C_Vector &, float, C_Entity *) = 0;
        virtual void GetLineIntersection(void)                                 = 0;
        virtual void FillImpactParticleInfo(void)                              = 0;
        virtual void InvalidateRelation(C_Entity *)                            = 0;
        virtual void GetCsInterface(void)                                      = 0;
        virtual void GetCreateCsInterface(void)                                = 0;
        virtual void UpdateIdleFX(float)                                       = 0;
        virtual void UpdateIdleRC(float)                                       = 0;

        // Non-virtual helper to get prefab type
        prefab::E_PREFAB_TYPE GetPrefabType() {
            using Fn = int(__fastcall *)(void *);
            auto vtable = *(uintptr_t **)this;
            return static_cast<prefab::E_PREFAB_TYPE>(((Fn)vtable[4])(this));
        }

        uint32_t GetId() const {
            return m_nIdAndType >> 8;
        }

        void SetId(uint32_t id) {
            m_nIdAndType = (id << 8) | (m_nIdAndType & 0xFF);
        }

        uint16_t GetGUID() const {
            return static_cast<uint16_t>(m_nIdAndType);
        }

        E_EntityType GetType() const {
            return static_cast<E_EntityType>(m_nIdAndType & 0xFF);
        }

        void SetType(E_EntityType type) {
            m_nIdAndType = (m_nIdAndType & 0xFFFFFF00) | static_cast<uint32_t>(type);
        }

        uint32_t GetFlags() const {
            return m_nFlags;
        }

        void SetFlags(uint32_t flags) {
            m_nFlags = flags;
        }

        uint64_t GetNameHash() const {
            return m_nNameHash;
        }

        ue::sys::core::C_SceneObject *GetSceneObject() const {
            return m_pSceneObject;
        }

        prefab::C_PrefabInitData *GetPrefabInitDataWrapper() {
            return &m_PrefabInitData;
        }

        void *GetPrefabInitData() {
            return m_PrefabInitData.GetConstInitData();
        }

        void GameInit();
        void GameDone();

        void Activate();
        void Deactivate();

        void Release();

      protected:
        void *m_pMessageReceiverList;                        // 0x08 - 0x10
        void *m_pMessageReceiverListEnd;                     // 0x10 - 0x18
        uint32_t m_nIdAndType;                               // 0x18 - 0x1C (type in low byte, id in upper 24 bits)
        uint32_t m_nFlags;                                   // 0x1C - 0x20
        float m_fUnk20;                                      // 0x20 - 0x24 (initialized to -1.0f)
        uint8_t m_bSceneLocked;                              // 0x24 - 0x25
        uint8_t m_bProcessingListeners;                      // 0x25 - 0x26
        E_UpdateRate m_eUpdateRate;                          // 0x26 - 0x27
        uint8_t m_nPad27;                                    // 0x27 - 0x28
        uint64_t m_nNameHash;                                // 0x28 - 0x30
        void *m_pScriptEventTree;                            // 0x30 - 0x38
        void *m_pScriptEventTreeEnd;                         // 0x38 - 0x40
        void *m_pLuaEventTree;                               // 0x40 - 0x48
        void *m_pLuaEventTreeEnd;                            // 0x48 - 0x50
        void *m_pEventListStart;                             // 0x50 - 0x58
        void *m_pEventListEnd;                               // 0x58 - 0x60
        void *m_pEventListCapacity;                          // 0x60 - 0x68
        void *m_pListenerListStart;                          // 0x68 - 0x70
        void *m_pListenerListEnd;                            // 0x70 - 0x78
        void *m_pListenerListCapacity;                       // 0x78 - 0x80
        void *m_pPendingListenerStart;                       // 0x80 - 0x88
        void *m_pPendingListenerEnd;                         // 0x88 - 0x90
        void *m_pPendingListenerCapacity;                    // 0x90 - 0x98
        void *m_pInitData;                                   // 0x98 - 0xA0
        prefab::C_PrefabInitData m_PrefabInitData;           // 0xA0 - 0xA8
        ue::sys::core::C_SceneObject *m_pSceneObject;        // 0xA8 - 0xB0
        void *m_pSceneObjectRef;                             // 0xB0 - 0xB8
        void *m_pSceneObjectRefData;                         // 0xB8 - 0xC0
        uint16_t m_nSceneObjectRefFlags;                     // 0xC0 - 0xC2
        uint8_t m_nPadC2[6];                                 // 0xC2 - 0xC8
        void *m_pSceneObjectFix;                             // 0xC8 - 0xD0
        uint32_t m_nDelSlot;                                 // 0xD0 - 0xD4
        uint8_t m_nPadD4[4];                                 // 0xD4 - 0xD8
        void *m_pSound;                                      // 0xD8 - 0xE0
    };

    static_assert(sizeof(C_Entity) == 0xE0, "C_Entity size mismatch");
} // namespace SDK
