#pragma once

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
        virtual void OnRecvMessage(/*C_EntityMessage**/ void *) = 0;
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

        unsigned int GetId() {
            return ((*(unsigned int *)((uintptr_t)this + 0x18)) >> 8);
        }

        void SetId(unsigned int id) {
            *(unsigned int *)((uintptr_t)this + 0x18) = (id << 8) | (*(unsigned int *)((uintptr_t)this + 0x18) & 0x000000FF);
        }

        uint16_t GetGUID() {
            return *(uint16_t *)((uintptr_t)this + 0x18);
        }

        E_EntityType GetType() {
            return (E_EntityType)((*(unsigned int *)((uintptr_t)this + 0x18)) & 0xFF);
        }

        void SetType(E_EntityType type) {
            *(unsigned int *)((uintptr_t)this + 0x18) = (*(unsigned int *)((uintptr_t)this + 0x18) & 0xFFFFFF00) | (uint32_t)type;
        }

        ue::sys::core::C_SceneObject *GetSceneObject() {
            return *(ue::sys::core::C_SceneObject **)((uintptr_t)this + 0xA8);
        }

        void GameInit();
        void GameDone();

        void Activate();
        void Deactivate();

        void Release();
    };
} // namespace SDK
