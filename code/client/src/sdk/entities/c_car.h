#pragma once

#include "c_actor.h"

namespace SDK {
    class C_Vehicle;
    class I_Human2;

    struct S_BaseSeat {
        enum E_BaseSeatStatus { OCCUPIED = 3, EMPTY = 4 };
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
        float GetFuelTankCapacity() const;

        void SetTransparency(float transparency);

        void SetSpeed(float speed);

        void SetVehicleDirty(float);

        void RestoreCar();

        int GetGear();

        float GetDamage() const;

        bool SetSeatStatus(I_Human2 *, unsigned int, S_BaseSeat::E_BaseSeatStatus);

        void ExplodeCar();
        void ExplodeCar(float, bool);

        C_Vehicle *GetVehicle() {
            return reinterpret_cast<C_Vehicle *>((uintptr_t)this + 0xF8);
        }

        bool IsEngineOn() const {
            uint64_t flags = *reinterpret_cast<uint64_t *>((uintptr_t)this + 0x1280);
            return (flags >> 2) & 1;
        }
    };
} // namespace SDK
