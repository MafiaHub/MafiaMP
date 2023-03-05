#pragma once

#include "../../ue/c_string.h"
#include "../../ue/sys/core/c_scene.h"
#include "../../ue/sys/utils/c_hash_name.h"
#include "../../streammap/flags/e_slot_type.h"
#include "e_slot_state.h"

namespace SDK {
    namespace mafia::streaming {
        class C_Slot {
          public:
            enum E_ParseDataState { E_PD_BEGIN, E_PD_SCRIPT_MACHINE, E_PD_SCRIPT_MACHINE_LOADING, E_PD_END, E_PD_FINISHED };

            enum E_DestroyDataState { E_DD_DESTROY_SCRIPT, E_DD_TEMP_ENTITIES, E_DD_UNLOAD_SDS, E_DD_FINISHED };

          private:
            uint8_t m_Unk0[16];
            uint8_t m_SlotWrappers[32 /*?*/]; // std::vector<mafia::streaming::I_SlotWrapper*>
            unsigned int m_Id;
            int m_Unk52;
            ue::sys::utils::C_HashName m_Name;
            streammap::flags::E_SlotType m_SlotType;
            int m_Unk68;
            unsigned int m_SizeMemory;
            int m_Priority;
            int m_Lock;
            E_SlotState m_SlotState;
            bool m_FailState;
            bool m_WaitForLoad;
            uint8_t m_Unk90[6];
            ue::sys::core::C_Scene *m_WaitForLoadScene;
            int m_Unk104;
            bool m_DoParseSDS;
            bool m_ParseBeforeDelete;
            bool m_WaitForDeferredIOResult;
            bool m_WaitForDefferedLoadResult;
            void *m_Unk112;
            ue::C_String *m_LoadedSDS;
            E_ParseDataState m_ParseDataState;
            E_DestroyDataState m_DestroyDataState;
            uint8_t m_Unk136[24];
            unsigned int m_Slot;
            uint8_t m_Unk164[12];

          public:
            ue::sys::utils::C_HashName GetName() {
                  return m_Name;
            }

            ue::C_String* GetLoadedSDS() {
                  return m_LoadedSDS;
            }
        };
    } // namespace mafia::streaming
    static_assert(sizeof(mafia::streaming::C_Slot) == 176, "Wrong size: mafia::streaming::C_Slot");
}
