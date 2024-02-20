#include "c_car.h"
#include "c_human_2.h"
#include "c_vehicle.h"

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

    bool C_Car::SetSeatStatus(I_Human2 *human, unsigned int seatID, S_BaseSeat::E_BaseSeatStatus status) {
        return hook::this_call<bool>(gPatterns.C_Car__SetSeatStatus, this, human, seatID, status);
    }

    float C_Car::GetActualFuel() const {
        uint64_t C_Motor = *(uint64_t *)(*(uint64_t *)((uint64_t)this + C_Motor_Offset) + 0x8);
        if (C_Motor) {
            return *(float *)(*(uint64_t *)C_Motor + 0x1FC);
        }
        return 0.0f;
    }

    int C_Car::GetColor(int kind) {
        return hook::this_call<int>(hook::get_opcode_address("E8 ? ? ? ? 41 89 47 34"), this, kind);
    }

    void C_Car::SetColor(int colorID1, int colorID2, bool metallic) {
        hook::this_call<void>(hook::get_opcode_address("E9 ? ? ? ? C3 E2 37"), this, colorID1, colorID2, metallic);
    }

    void C_Car::SwitchColor(bool randomize) {
        hook::this_call<void>(reinterpret_cast<uint64_t>(hook::get_pattern("40 56 48 83 EC 20 4C 8B 81 ? ? ? ?")), this, randomize);
    }

    int C_Car::GetWindowTint() {
        return hook::this_call<int>(hook::get_opcode_address("E8 ? ? ? ? 8D 50 01 48 8B 43 48"), this);
    }

    void C_Car::SetWindowTint(int colorID) {
        hook::this_call<void>(hook::get_opcode_address("E8 ? ? ? ? EB 65 48 8D 8B ? ? ? ?"), this, colorID);
    }

    void C_Car::SetInteriorColorsSet(int setID) {
        hook::this_call<void>(hook::get_opcode_address("E8 ? ? ? ? EB 71 8B D6"), this, setID);
    }

    void C_Car::SwitchInteriorColor(bool randomize) {
        hook::this_call<void>(reinterpret_cast<uint64_t>(hook::get_pattern("40 57 48 83 EC 30 4C 8B 81 ? ? ? ?")), this, randomize);
    }

    void C_Car::SetWheelColor(int rimColorID, int tireColorID) {
        hook::this_call<void>(hook::get_opcode_address("E8 ? ? ? ? 80 7C 24 ? ? 74 08 48 8B CF E8 ? ? ? ? 45 84 ED"), this, rimColorID, tireColorID);
    }

    void C_Car::SetPainting(char const *paintName, bool stream) {
        hook::this_call<void>(reinterpret_cast<uint64_t>(hook::get_pattern("48 8B C4 55 56 48 8D 68 ? 48 81 EC ? ? ? ? 48 89 58 ? 48 89 78 ? 33 FF")), this, paintName, stream);
    }

    void C_Car::SetActualFuel(float fuel) {
        hook::this_call<void>(gPatterns.C_Car__SetActualFuel, this, fuel);

        // TODO: fix me, doesn't work yet
        // uint64_t C_Motor = *(uint64_t *)(*(uint64_t *)((uint64_t)this + C_Motor_Offset) + 0x8);
        // if (C_Motor) {
        //     hook::this_call(gPatterns.C_Motor__SetFuel, C_Motor, fuel);
        // }
    }

    float C_Car::GetFuelTankCapacity() const {
        return hook::this_call<float>(gPatterns.C_Car__GetFuelTankCapacity, this);
    }

    void C_Car::SetTransparency(float transparency) {
        hook::this_call<void>(gPatterns.C_Car__SetTransparency, this, transparency);
    }

    void C_Car::SetSpeed(float speed) {
        hook::this_call<void>(gPatterns.C_Car__SetSpeed, this, speed);
    }

    void C_Car::SetVehicleDirty(float dirt) {
        hook::this_call<void>(gPatterns.C_Car__SetVehicleDirty, this, dirt);
    }

    void C_Car::RestoreCar() {
        hook::this_call<void>(gPatterns.C_Car__RestoreCar, this);
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
