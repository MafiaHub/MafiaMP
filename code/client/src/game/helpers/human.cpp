#include "human.h"

#include <glm/glm.hpp>

#include "game/overrides/character_controller.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_human_2.h"

namespace MafiaMP::Game::Helpers {
    uint8_t Human::GetHealthPercent(SDK::C_Human2 *human) {
        float fHealth    = human->GetHumanScript()->GetHealth();
        float fHealthMax = human->GetHumanScript()->GetHealthMax();
        return (uint8_t)glm::clamp(fHealth / fHealthMax, 0.0f, 100.0f);
    }

    void Human::SetHealthPercent(SDK::C_Human2 *human, float health) {
        float fHealthMax = human->GetHumanScript()->GetHealthMax();
        return human->GetHumanScript()->SetHealth(glm::clamp(health * fHealthMax, 0.0f, fHealthMax));
    }

    bool Human::PutIntoCar(MafiaMP::Game::Overrides::CharacterController *charController, SDK::C_Car *car, int seat, bool force) {
        if (!car) {
            return false;
        }

        SDK::C_Actor *act = *(SDK::C_Actor **)((uintptr_t)car + 0xA8);
        return charController->TriggerActorAction(act, SDK::E_AA_ENTER_CAR, seat, force, false);
    }

    bool Human::RemoveFromCar(MafiaMP::Game::Overrides::CharacterController *charController, SDK::C_Car *car, bool force) {
        if (!car) {
            return false;
        }

        SDK::C_Actor *act = *(SDK::C_Actor **)((uintptr_t)car + 0xA8);
        return charController->TriggerActorAction(act, SDK::E_AA_LEAVE_CAR, 0, force, false);
    }
    void Human::AddWeapon(SDK::C_Human2 *human, int weapon, int ammo) {
        if (!human)
            return;

        human->m_pHumanInventory->AddWeapon(weapon, ammo);
    }
} // namespace MafiaMP::Game::Helpers
