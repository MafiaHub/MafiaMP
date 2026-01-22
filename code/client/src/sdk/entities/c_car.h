#pragma once

#include "c_actor_vehicle.h"
#include "c_car_tuning_manager.h"
#include "../ue/game/vehicle/c_vehicle.h"

namespace SDK {
    class I_Human2;

    struct S_BaseSeat {
        enum E_BaseSeatStatus { OCCUPIED = 3, EMPTY = 4 };
    };

    class C_Car: public C_ActorVehicle {
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

        ue::game::vehicle::C_Vehicle *GetVehicle() {
            return reinterpret_cast<ue::game::vehicle::C_Vehicle *>((uintptr_t)this + 0xF8);
        }

        bool IsEngineOn() const {
            uint32_t flags = *reinterpret_cast<uint32_t *>((uintptr_t)this + 0x1280);
            return (flags >> 2) & 1;
        }

        // Tuning system
        C_CarTuningManager *GetTuningManager() {
            return reinterpret_cast<C_CarTuningManager *>((uintptr_t)this + 0x1B20);
        }

        const C_CarTuningManager *GetTuningManager() const {
            return reinterpret_cast<const C_CarTuningManager *>((uintptr_t)this + 0x1B20);
        }

        void InstallTuningItems();
        void InitVisualTuning();

        // Color/painting
        void SetColor(uint32_t color1, uint32_t color2, bool metallic);
        void SetInteriorColorsSet(uint32_t setId);
        void SetWindowTint(uint32_t tintId);
        void SetPainting(const char *paintingName, uint8_t arg);
    };
} // namespace SDK
