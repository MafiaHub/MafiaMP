#include "c_car.h"
#include "c_vehicle.h"
#include "c_human_2.h"

#include <utils/hooking/hooking.h>
#include "../patterns.h"

#ifdef NONSTEAM_SUPPORT
constexpr uint64_t C_Motor_Offset = 0x468;
#else
constexpr uint64_t C_Motor_Offset = 0x458;
#endif

namespace SDK {

    void C_Car::Lock() {
        hook::this_call(gPatterns.C_Car__Lock, this);
    }

    void C_Car::Unlock() {
        hook::this_call(gPatterns.C_Car__Unlock, this);
    }

    void C_Car::LockEntryPoints() {
        hook::this_call(gPatterns.C_Car__LockEntryPoints, this);
    }

    void C_Car::UnlockEntryPoints() {
        hook::this_call(gPatterns.C_Car__UnlockEntryPoints, this);
    }

    void C_Car::SetSPZText(const char *spz, bool unk1) {
        GetVehicle()->SetSPZText(spz, unk1);
    }

    const char *C_Car::GetSPZText() {
        return GetVehicle()->GetSPZText();
    }

    void C_Car::OpenHood() {
        hook::this_call<void>(gPatterns.C_Car__OpenHood, this);
    }

    void C_Car::CloseHood() {
        hook::this_call<void>(gPatterns.C_Car__CloseHood, this);
    }

    void C_Car::OpenTrunk() {
        hook::this_call<void>(gPatterns.C_Car__OpenTrunk, this);
    }

    void C_Car::CloseTrunk() {
        hook::this_call<void>(gPatterns.C_Car__CloseTrunk, this);
    }

    float C_Car::GetDamage() const {
        return hook::this_call<float>(gPatterns.C_Car__GetDamage, this);
    }

    void C_Car::SetMotorDamage(float damage) {
        hook::this_call<void>(gPatterns.C_Car__SetMotorDamage, this, damage);
    }

    float C_Car::GetMotorDamage() const {
        return hook::this_call<float>(gPatterns.C_Car__GetMotorDamage, this);
    }

    void C_Car::SetActualFuel(float fuel) {
        uint64_t C_Motor = *(uint64_t *)(*(uint64_t *)((uint64_t)this + C_Motor_Offset) + 0x8);
        if (C_Motor) {
            hook::this_call(gPatterns.C_Motor__SetFuel, C_Motor, fuel);
        }
    }

    bool C_Car::SetSeatStatus(I_Human2 *human, unsigned int seatID, S_BaseSeat::E_BaseSeatStatus status){
        return hook::this_call<bool>(gPatterns.C_Car__SetSeatStatus, this, human, seatID, status);
    }

    float C_Car::GetActualFuel() const {
        uint64_t C_Motor = *(uint64_t *)(*(uint64_t *)((uint64_t)this + C_Motor_Offset) + 0x8);
        if (C_Motor) {
            return *(float *)(*(uint64_t *)C_Motor + 0x1FC);
        }
        return 0.0f;
    }

    void C_Car::SetTransparency(float transparency) {
        hook::this_call<void>(gPatterns.C_Car__SetTransparency, this, transparency);
    }

    void C_Car::SetSpeed(float speed) {
        hook::this_call<void>(gPatterns.C_Car__SetSpeed, this, speed);
    }

    void C_Car::PosefujZimuVShopu(float dirt) {
        hook::this_call<void>(gPatterns.C_Car__PosefujZimuVShopu, this, dirt);
    }

    void C_Car::Restore(bool arg1, bool arg2, bool arg3) {
        hook::this_call<void>(gPatterns.C_Car__Restore, this, arg1, arg2, arg3);
    }

    int C_Car::GetGear() {
        uint64_t C_Motor = *(uint64_t *)(*(uint64_t *)((uint64_t)this + C_Motor_Offset) + 0x8);
        if (C_Motor) {
            return *(uint32_t *)(*(uint64_t *)C_Motor + 0xB8) - *(uint32_t *)(*(uint64_t *)C_Motor + 0x19C);
        }
        return 0;
    }

    void C_Car::ExplodeCar() {
        hook::this_call(gPatterns.C_Car__ExplodeCar, this);
    }

    void C_Car::ExplodeCar(float unk1, bool unk2) {
        hook::this_call(gPatterns.C_Car__ExplodeCar_2, this, unk1, unk2);
    }
} // namespace SDK
