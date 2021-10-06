#pragma once

#include "../../c_game.h"
#include "../../entities/c_entity_list.h"
#include "../../entities/player/c_player_model_manager.h"

#include "../streaming/c_streaming_module.h"

namespace SDK {
    namespace mafia::framework {
        class C_MafiaFrameworkInterfaces {
          private:
            C_Game *m_pGame;                                         // 0
            void *m_pMission;                                        // 8
            C_EntityList *m_pEntityList;                             // 10
            void *m_pSDSScriptMachineManager;                        // 18
            void *m_pSDSLoadUnloadNotify;                            // 20
            void *m_pActorActionListenerRegistrator;                 // 28
            C_PlayerModelManager *m_pPlayerModelManager;             // 30
            void *m_pScriptDataStorage;                              // 38
            void *m_pPlayerVehicleStreamingTickedModule;             // 40
            mafia::streaming::C_StreamingModule *m_pStreamingModule; // 48
            void *m_pGlobalGameDataManager;

          public:
            C_Game *GetGame() {
                return m_pGame;
            }
            void *GetMission() {
                return m_pMission;
            }
            C_EntityList *GetEntityList() {
                return m_pEntityList;
            }
            void *GetSDSScriptMachineManager() {
                return m_pSDSScriptMachineManager;
            }
            void *GetSDSLoadUnloadNotify() {
                return m_pSDSLoadUnloadNotify;
            }
            void *GetActorActionListenerRegistrator() {
                return m_pActorActionListenerRegistrator;
            }
            C_PlayerModelManager *GetPlayerModelManager() {
                return m_pPlayerModelManager;
            }
            void *GetScriptDataStorage() {
                return m_pScriptDataStorage;
            }
            void *GetPlayerVehicleStreaming() {
                return m_pPlayerVehicleStreamingTickedModule;
            }
            mafia::streaming::C_StreamingModule *GetStreamingModule() {
                auto v1     = *(uint64_t *)((uint64_t)this + 0x48);
                auto result = v1 + 8;
                if (!v1)
                    result = 0;

                return reinterpret_cast<mafia::streaming::C_StreamingModule *>(result);
            }
            void *GetGlobalGameDataManager() {
                return m_pGlobalGameDataManager;
            }
        };
    } // namespace mafia::framework
} // namespace SDK
