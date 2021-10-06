#pragma once

#include "c_slot.h"
#include "../../streammap/flags/e_slot_type.h"
#include "../../ue/sys/core/c_scene.h"

namespace SDK {
    namespace mafia::streaming {
        class I_SlotWrapper {
          public:
            virtual ~I_SlotWrapper() {
            }
            virtual void *GetClassTypeInfo() {
                return nullptr;
            }
            virtual void *GetTypeInfo() {
                return nullptr;
            }
            virtual const char *GetWrapperName() {
                return nullptr;
            }
            virtual bool CanShareSlotForWrapper(I_SlotWrapper const *) {
                return false;
            }
            virtual int CanDisconnectSlotForWrapper(I_SlotWrapper const *) {
                return 0;
            }
            virtual bool DisconnectSlotForWrapper(I_SlotWrapper const *) {
                return false;
            }
            virtual int GetLoadPriority() {
                return 0;
            }
            virtual bool ConnectToQuota(char const *, streammap::flags::E_SlotType, int) {
                return false;
            }
            virtual bool MoveToQuota(char const *) {
                return false;
            }
            virtual void *Disconnect() {
                return nullptr;
            }
            virtual void OnEvent(/*mafia::streaming::E_EventType*/ int) {
            }
            virtual void ConnectToSlot(C_Slot *) {
            }
            virtual void DisconnectFromSlot() {
            }
            virtual bool ConnectToQuotaUsingExistingSlot(char const *) {
                return false;
            }
            virtual void DisconnectFromQuota() {
            }
        };

        class C_SlotWrapperBase: public I_SlotWrapper {
          protected:
            int m_Lock;
            int m_Unk12;
            C_Slot *m_pSlot;
        };

        class C_SlotWrapper: public C_SlotWrapperBase {
          public:
            bool LoadData(char const *, ue::sys::core::C_Scene *, unsigned int, char const *, bool *, bool);
        };
    } // namespace mafia::streaming
    static_assert(sizeof(mafia::streaming::C_SlotWrapper) == 24, "Wrong size: mafia::streaming::C_SlotWrapperBase");
}
