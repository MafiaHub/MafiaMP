#include "human.h"

#include "sdk/entities/c_human_2.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/human/c_human_script.h"
#include "game/overrides/character_controller.h"

namespace MafiaMP::Game::Helpers {
    uint8_t Human::GetHealthPercent(SDK::C_Human2 *human){
        float fHealth        = human->GetHumanScript()->GetHealth();
        float fHealthMax     = human->GetHumanScript()->GetHealthMax();
        float fHealthPercent = std::clamp((fHealth / fHealthMax) * 100.f, 0.0f, 100.0f);
        return (uint8_t)fHealthPercent;
    }

    bool Human::PutIntoCar(MafiaMP::Game::Overrides::CharacterController *charController, SDK::C_Car *car, int seat, bool force) {
        if (!car) return false;
        SDK::C_Actor *act = *(SDK::C_Actor **)((uintptr_t)car + 0xA8);
        return charController->TriggerActorAction(act, SDK::E_AA_ENTER_CAR, seat, force, false);
    }

    bool Human::RemoveFromCar(MafiaMP::Game::Overrides::CharacterController *charController, SDK::C_Car *car, bool force) {
        if (!car) return false;
        SDK::C_Actor *act = *(SDK::C_Actor **)((uintptr_t)car + 0xA8);
        return charController->TriggerActorAction(act, SDK::E_AA_LEAVE_CAR, 0, force, false);
    }
}
