#pragma once

#include "c_actor.h"

namespace SDK {
    class C_Vehicle;
    class I_Human2;

    struct S_BaseSeat {
        enum E_BaseSeatStatus {
            EMPTY = 4
        };
    };

    class C_Car: public C_Actor {
      public:
        void Lock();
        void Unlock();

        void LockEntryPoints();
        void UnlockEntryPoints();

        void SetSPZText(const char *spz, bool unk1);
        const char *GetSPZText();

        void OpenHood();
        void CloseHood();

        void OpenTrunk();
        void CloseTrunk();

        void SetMotorDamage(float damage);
        float GetMotorDamage() const;

        void SetActualFuel(float fuel);
        float GetActualFuel() const;

        void SetTransparency(float transparency);
        void SetSpeed(float speed);

        /*
         * PosefujZimuVShopu - same function as SetVehicleDirt
         */
        void PosefujZimuVShopu(float dirt);
        void Restore(bool arg1, bool arg2, bool arg3);

        int GetGear();
        float GetDamage() const;

        bool SetSeatStatus(I_Human2 *, unsigned int, S_BaseSeat::E_BaseSeatStatus);

        C_Vehicle *GetVehicle() {
            return reinterpret_cast<C_Vehicle *>((uintptr_t)this + 0xF8);
        }
    };
} // namespace SDK
