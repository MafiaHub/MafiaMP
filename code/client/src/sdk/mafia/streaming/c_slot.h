#pragma once

#include "../../ue/c_string.h"
#include "../../ue/sys/core/c_scene.h"
#include "../../ue/sys/core/c_scene_object.h"
#include "../../ue/sys/utils/c_hash_name.h"
#include "../../streammap/flags/e_slot_type.h"
#include "e_slot_state.h"

#include <functional>
#include <string>
#include <vector>

namespace SDK {
    namespace mafia::streaming {
        class C_Slot {
          public:
            enum E_ParseDataState { E_PD_BEGIN, E_PD_SCRIPT_MACHINE, E_PD_SCRIPT_MACHINE_LOADING, E_PD_END, E_PD_FINISHED };

            enum E_DestroyDataState { E_DD_DESTROY_SCRIPT, E_DD_TEMP_ENTITIES, E_DD_UNLOAD_SDS, E_DD_FINISHED };

          private:
            uint8_t m_Unk0[16];                   // 0x00 - 0x10
            uint8_t m_SlotWrappers[32];           // 0x10 - 0x30 std::vector<I_SlotWrapper*>
            unsigned int m_Id;                    // 0x30 - 0x34
            int m_Unk52;                          // 0x34 - 0x38
            ue::sys::utils::C_HashName m_Name;    // 0x38 - 0x40
            streammap::flags::E_SlotType m_SlotType; // 0x40 - 0x44
            int m_Unk68;                          // 0x44 - 0x48
            unsigned int m_SizeMemory;            // 0x48 - 0x4C
            int m_Priority;                       // 0x4C - 0x50
            int m_Lock;                           // 0x50 - 0x54
            E_SlotState m_SlotState;              // 0x54 - 0x58
            bool m_FailState;                     // 0x58 - 0x59
            bool m_WaitForLoad;                   // 0x59 - 0x5A
            uint8_t m_Unk90[6];                   // 0x5A - 0x60
            ue::sys::core::C_Scene *m_WaitForLoadScene; // 0x60 - 0x68
            int m_Unk104;                         // 0x68 - 0x6C
            bool m_DoParseSDS;                    // 0x6C - 0x6D
            bool m_ParseBeforeDelete;             // 0x6D - 0x6E
            bool m_WaitForDeferredIOResult;       // 0x6E - 0x6F
            bool m_WaitForDefferedLoadResult;     // 0x6F - 0x70
            void *m_Unk112;                       // 0x70 - 0x78
            ue::C_String *m_LoadedSDS;            // 0x78 - 0x80
            E_ParseDataState m_ParseDataState;    // 0x80 - 0x84
            E_DestroyDataState m_DestroyDataState;// 0x84 - 0x88
            void **m_SceneObjectsStart;           // 0x88 - 0x90
            void **m_SceneObjectsEnd;             // 0x90 - 0x98
            uint8_t m_Unk152[8];                  // 0x98 - 0xA0
            unsigned int m_Slot;                  // 0xA0 - 0xA4
            uint8_t m_Unk164[12];                 // 0xA4 - 0xB0

          public:
            ue::sys::utils::C_HashName GetName() {
                return m_Name;
            }

            ue::C_String *GetLoadedSDS() {
                return m_LoadedSDS;
            }

            void **GetSceneObjectsStart() const {
                return m_SceneObjectsStart;
            }

            void **GetSceneObjectsEnd() const {
                return m_SceneObjectsEnd;
            }

            size_t GetSceneObjectCount() const {
                if (!m_SceneObjectsStart || !m_SceneObjectsEnd) {
                    return 0;
                }
                return (m_SceneObjectsEnd - m_SceneObjectsStart);
            }

            ue::sys::core::C_SceneObject *GetSceneObject(size_t index) const {
                if (index >= GetSceneObjectCount()) {
                    return nullptr;
                }
                return reinterpret_cast<ue::sys::core::C_SceneObject *>(m_SceneObjectsStart[index]);
            }

            void EnumerateSceneObjects(const std::function<bool(size_t index, ue::sys::core::C_SceneObject *obj)> &callback) const {
                const size_t count = GetSceneObjectCount();
                for (size_t i = 0; i < count; ++i) {
                    auto *obj = GetSceneObject(i);
                    if (obj && !callback(i, obj)) {
                        break;
                    }
                }
            }

            std::vector<std::string> GetSceneObjectNames() const {
                std::vector<std::string> names;
                EnumerateSceneObjects([&](size_t, ue::sys::core::C_SceneObject *obj) {
                    const char *name = obj->GetName();
                    if (name && name[0] != '\0') {
                        names.push_back(name);
                    }
                    return true;
                });
                return names;
            }
        };
    } // namespace mafia::streaming
    static_assert(sizeof(mafia::streaming::C_Slot) == 176, "Wrong size: mafia::streaming::C_Slot");
}
