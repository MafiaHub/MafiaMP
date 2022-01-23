#pragma once

#include "c_entity.h"
#include "c_actor.h"

namespace SDK {
    class C_EntityList {
      public:
        virtual ~C_EntityList()                                                                                      = default;
        virtual void *GetClassTypeInfo(void)                                                                         = 0;
        virtual void *GetTypeInfo(void)                                                                              = 0;
        virtual void AddEntity(C_Entity *)                                                                           = 0;
        virtual void RemoveEntity(C_Entity *)                                                                        = 0;
        virtual void UpdateEntityInNamesTable(/*C_Entity*, ue::sys::utils::C_HashName, ue::sys::utils::C_HashName*/) = 0;
        virtual void RemoveEntityFromNamesTable(C_Entity *)                                                          = 0;
        virtual unsigned int GetEntityCount(void)                                                                    = 0;
        virtual C_Entity *GetEntityByIndex(int)                                                                      = 0;
        virtual C_Entity *GetEntityByGuid(unsigned int GUID, bool arg2)                                              = 0;
        virtual C_Entity *GetEntityByName(/*ue::sys::utils::C_HashName, bool*/) = 0;
        virtual C_Actor *GetActorByGuid(unsigned int, bool)                                                          = 0;
        virtual void ProcessEventToListeners(/*C_Entity*, E_EntityEvents*/) = 0;
        virtual void RegisterListener(/*I_EntityListener**/) = 0;
        virtual void UnregisterListener(/*I_EntityListener**/) = 0;
    };

    C_EntityList *GetEntityList();
}
