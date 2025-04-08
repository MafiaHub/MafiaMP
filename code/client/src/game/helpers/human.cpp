#include "human.h"

#include <glm/glm.hpp>

#include "game/overrides/character_controller.h"
#include "sdk/entities/c_car.h"
#include "sdk/entities/c_human_2.h"

#include "sdk/c_game.h"

namespace MafiaMP::Game::Helpers {
    uint8_t Human::GetHealthPercent(SDK::C_Human2 *human) {
        float fHealth    = human->GetHumanScript()->GetHealth();
        float fHealthMax = human->GetHumanScript()->GetHealthMax();
        return (uint8_t)std::clamp((fHealth / fHealthMax) * 100.f, 0.0f, 100.0f);
    }

    void Human::SetHealthPercent(SDK::C_Human2 *human, float health) {
        float fHealthMax = human->GetHumanScript()->GetHealthMax();
        return human->GetHumanScript()->SetHealth(std::clamp(health * fHealthMax, 0.0f, fHealthMax));
    }

    bool Human::PutIntoCar(MafiaMP::Game::Overrides::CharacterController *charController, SDK::C_Car *car, int seat, bool force) {
        if (!car) {
            return false;
        }

        const auto pLocalPlayer = SDK::GetGame()->GetActivePlayer();
        if (!pLocalPlayer) {
            return false;
        }

        // Specific code for local player that has no character controller bound
        if (!charController || charController->GetCharacter()->GetType() == SDK::E_EntityType::E_ENTITY_PLAYER) {
            SDK::ue::C_CntPtr<uintptr_t> syncObject;
            reinterpret_cast<SDK::C_Human2 *>(pLocalPlayer)->GetHumanScript()->GetOnVehicle(syncObject, car, seat, true, true, SDK::E_HumanMoveMode::E_HMM_WALK, force);
            return true;
        }

        SDK::C_Actor *act = *(SDK::C_Actor **)((uintptr_t)car + 0xA8);
        return charController->TriggerActorAction(act, SDK::E_AA_ENTER_CAR, seat, force, false);
    }

    bool Human::RemoveFromCar(MafiaMP::Game::Overrides::CharacterController *charController, SDK::C_Car *car, bool force) {
        if (!car) {
            return false;
        }

        const auto pLocalPlayer = SDK::GetGame()->GetActivePlayer();
        if (!pLocalPlayer) {
            return false;
        }

        // Specific code for local player that has no character controller bound
        if (!charController || charController->GetCharacter()->GetType() == SDK::E_EntityType::E_ENTITY_PLAYER) {
            SDK::ue::C_CntPtr<uintptr_t> syncObject;
            reinterpret_cast<SDK::C_Human2 *>(pLocalPlayer)->GetHumanScript()->ScrDoAction(syncObject, car, SDK::E_AA_LEAVE_CAR, 0, false, true, SDK::E_HumanMoveMode::E_HMM_WALK, true);
            return true;
        }

        SDK::C_Actor *act = *(SDK::C_Actor **)((uintptr_t)car + 0xA8);
        return charController->TriggerActorAction(act, SDK::E_AA_LEAVE_CAR, 0, force, false);
    }
    void Human::AddWeapon(SDK::C_Human2 *human, int weapon, int ammo) {
        if (!human)
            return;

        human->GetInventoryWrapper()->AddWeapon(weapon, ammo);
    }
} // namespace MafiaMP::Game::Helpers
