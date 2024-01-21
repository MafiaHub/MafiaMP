#include "c_door.h"

#include "../patterns.h"

namespace SDK {
    void C_Door::Lock() {
        hook::this_call(gPatterns.C_Door__Lock, this);
    }

    void C_Door::AILock() {
        hook::this_call(gPatterns.C_Door__AILock, this);
    }

    void C_Door::Unlock() {
        hook::this_call(gPatterns.C_Door__Unlock, this);
    }

    void C_Door::AIUnlock() {
        hook::this_call(gPatterns.C_Door__AIUnlock, this);
    }

    bool C_Door::IsLocked() const {
        return m_iDoorState != 0;
    }

    void C_Door::StartLockpick() {
        hook::this_call(gPatterns.C_Door__StartLockpick, this);
    }

    void C_Door::StopLockpick() {
        hook::this_call(gPatterns.C_Door__StopLockpick, this);
    }

    bool C_Door::IsLockpicking() const {
        return m_iDoorState == 4;
    }

    void C_Door::Kick(const ue::sys::math::C_Vector &pos) {
        hook::this_call(gPatterns.C_Door__Kick, this, pos);
    }

    void C_Door::Open(ue::sys::math::C_Vector const &arg1, bool arg2, unsigned int arg3) {
        hook::this_call(gPatterns.C_Door__Kick, this, arg1, arg2, arg3);
    }

    void C_Door::Close() {
        hook::this_call(gPatterns.C_Door__Close, this);
    }

    void C_Door::ToggleOpen(ue::sys::math::C_Vector const &arg1, bool arg2) {
        hook::this_call(gPatterns.C_Door__ToggleOpen, this, arg1, arg2);
    }

    bool C_Door::IsClosed() const {
        return m_iCloseState == 1;
    }

    void C_Door::EnableAction() {
        hook::this_call(gPatterns.C_Door__EnableAction, this);
    }

    void C_Door::DisableAction() {
        hook::this_call(gPatterns.C_Door__DisableAction, this);
    }
} // namespace SDK
