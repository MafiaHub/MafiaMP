#pragma once

#include "c_actor.h"

namespace SDK {
    class C_Vehicle;
    class I_Human2;

    struct S_BaseSeat {
        enum E_BaseSeatStatus {
            OCCUPIED = 3,
            EMPTY    = 4
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

        /**
         * @param kind (primary: 0, secondary 1)
         */
        int GetColor(int kind);

        void SetColor(int colorID1, int colorID2, bool metallic);

        /**
         * @param randomize If false, the default will be used.
         */
        void SwitchColor(bool randomize);

        int GetWindowTint();

        void SetWindowTint(int colorID);

        /**
         * Custom method
         */
        int GetInteriorColorsSet() {
            return *(int *)(((uintptr_t)this) + 7964);
        };

        void SetInteriorColorsSet(int setID);

        /**
         * @param randomize If false, the default will be used.
         */
        void SwitchInteriorColor(bool randomize);

        /**
         * Custom method
         */
        void GetWheelColor(int *rimColorID, int *tireColorID) {
            *rimColorID  = *(int *)(((uintptr_t)this) + 7968);
            *tireColorID = *(int *)(((uintptr_t)this) + 7972);
        }

        void SetWheelColor(int rimColorID, int tireColorID);

        void SetPainting(char const *paintName, bool stream);

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
            // TODO: move to class fields
            return reinterpret_cast<C_Vehicle *>((uintptr_t)this + 0xF8);
        }

        bool IsEngineOn() const {
            // TODO: move to class fields
            uint64_t flags = *reinterpret_cast<uint64_t *>((uintptr_t)this + 0x1280);
            return (flags >> 2) & 1;
        }
    };
} // namespace SDK
