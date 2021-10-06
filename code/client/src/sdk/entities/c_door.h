#pragma once

#include <cstdint>

#include "../ue/sys/math/c_vector.h"

namespace SDK {
    class C_Door {
      public:
        void Lock();
        void AILock();
        void Unlock();
        void AIUnlock();
        bool IsLocked() const;
        void StartLockpick();
        void StopLockpick();
        bool IsLockpicking() const;
        void Kick(const ue::sys::math::C_Vector &);
        void Open(ue::sys::math::C_Vector const &, bool, unsigned int);
        void Close();
        void ToggleOpen(ue::sys::math::C_Vector const &, bool);
        bool IsClosed() const;
        void EnableAction();
        void DisableAction();

      private:
        char pad_1[0x4A0];
        uint32_t m_iDoorState; // 0x04A0 - 0x04A4
        char pad_2[0x28];
        uint32_t m_iCloseState;
    };
}
