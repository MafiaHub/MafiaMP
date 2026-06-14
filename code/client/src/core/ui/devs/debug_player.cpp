#include "debug_player.h"

#include "sdk/c_player_teleport_module.h"
#include "sdk/entities/c_player_2.h"

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI::Devs {
    void DebugPlayer::OnOpen() {}

    void DebugPlayer::OnClose() {}

    void DebugPlayer::OnUpdate() {
        /**
         * TODO
         * - Implement GetScale/SetScale => see https://github.com/MafiaHub/MafiaMP/issues/49
         * - Implement kill player => see https://github.com/MafiaHub/MafiaMP/issues/52
         */

        const auto pActivePlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!pActivePlayer) {
            return;
        }

        auto windowContent = [&]() {
            auto position = pActivePlayer->GetPos();
            if (ImGui::DragFloat3("Pos", (float *)&position, 0.1f, -4500.0f, 4500.0f)) {
                pActivePlayer->SetPos(position);
            }

            auto dir = pActivePlayer->GetDir();
            if (ImGui::DragFloat3("Dir", (float *)&dir, 0.01f, -1.0f, 1.0f)) {
                pActivePlayer->SetDir(dir);
            }

            auto rot = pActivePlayer->GetRot();
            if (ImGui::DragFloat4("Rot", (float *)&rot, 0.01f, -1.0f, 1.0f)) {
                pActivePlayer->SetRot(rot);
            }

            float transparency = pActivePlayer->GetTransparency();
            if (ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f)) {
                pActivePlayer->SetTransparency(transparency);
            }

            uint64_t defaultSpawnProfile                     = 335218123840277515;
            ImGuiInputTextFlags_ defaultSpawnProfileGuiFlags = ImGuiInputTextFlags_ReadOnly;
            ImGui::InputScalar("Default SpawnProfile", ImGuiDataType_U64, &defaultSpawnProfile, NULL, &defaultSpawnProfileGuiFlags);

            uint64_t spawnProfile = Game::Helpers::Controls::GetLocalPlayerSpawnProfile();
            if (ImGui::InputScalar("SpawnProfile", ImGuiDataType_U64, &spawnProfile, NULL)) {
                Game::Helpers::Controls::PlayerChangeSpawnProfile(spawnProfile);
            }

            auto weaponController = pActivePlayer->GetHumanWeaponController();
            if (weaponController) {
                ImGui::Text("RightHandWeaponID: %i\n", weaponController->GetRightHandWeaponID());
                ImGui::Text("IsThrownWeapon: %s\n", weaponController->IsThrownWeapon() ? "true" : "false");

                SDK::ue::sys::math::C_Vector aimDirection;
                weaponController->GetAimDir(&aimDirection);
                ImGui::DragFloat3("Aim Direction", (float *)&aimDirection, 0.1f, -4500.0f, 4500.0f);

                ImGui::Text("IsAiming: %s\n", weaponController->IsAiming() ? "true" : "false");
                ImGui::Text("IsFirePressed: %s\n", weaponController->m_bFirePressed ? "true" : "false");
                ImGui::Text("IsFireHeld: %s\n", weaponController->m_bFireHeld ? "true" : "false");
            }
            else {
                ImGui::Text("Weapon Controller: N/A");
            }

            ImGui::Text("IsDeath: %s\n", pActivePlayer->IsDeath() ? "true" : "false");
            ImGui::Text("IsInVehicle: %s\n", pActivePlayer->IsInVehicle() ? "true" : "false");
            ImGui::Text("IsInCar: %s\n", pActivePlayer->IsInCar() ? "true" : "false");
            ImGui::Text("IsInBoat: %s\n", pActivePlayer->IsInBoat() ? "true" : "false");

            ImGui::Text("IsCrouch: %s\n", pActivePlayer->IsCrouch() ? "true" : "false");

            ImGui::Text("IsInCover: %s\n", pActivePlayer->IsInCover() ? "true" : "false");
            ImGui::Text("IsInCoverFull(true): %s\n", pActivePlayer->IsInCoverFull(true) ? "true" : "false");
            ImGui::Text("IsInCoverFull(false): %s\n", pActivePlayer->IsInCoverFull(false) ? "true" : "false");

            bool lockedControls = Game::Helpers::Controls::AreControlsLocked();
            if (ImGui::Checkbox("Lock Controls", &lockedControls)) {
                Game::Helpers::Controls::Lock(lockedControls);
            }

            bool demigod = pActivePlayer->GetHumanScript()->IsDemigod();
            if (ImGui::Checkbox("Demigod", &demigod)) {
                pActivePlayer->GetHumanScript()->SetDemigod(demigod);
            }

            bool invulnerable = pActivePlayer->GetHumanScript()->GetInvulnerabilityByScript();
            if (ImGui::Checkbox("Invulnerable", &invulnerable)) {
                pActivePlayer->GetHumanScript()->SetInvulnerabilityByScript(invulnerable);
            }

            float health = pActivePlayer->GetHumanScript()->GetHealth();
            if (ImGui::SliderFloat("Health", &health, 0.0f, pActivePlayer->GetHumanScript()->GetHealthMax())) {
                pActivePlayer->GetHumanScript()->SetHealth(health);
                (health);
            }

            // SetHealthMax is explicitly disabled for the player
            ImGui::Text("Health Max: %f\n", pActivePlayer->GetHumanScript()->GetHealthMax());

            {
                if (ImGui::Button("Heal")) {
                    float healthMax = pActivePlayer->GetHumanScript()->GetHealthMax();
                    pActivePlayer->GetHumanScript()->SetHealth(healthMax);
                }

                ImGui::SameLine();

                if (ImGui::Button("Heal to -1.0f")) {
                    pActivePlayer->GetHumanScript()->SetHealth(-1.0f); // Doesn't kill the player
                }
            }

            {
                if (ImGui::Button("Give weapons")) {
                    pActivePlayer->GetInventoryWrapper()->AddWeapon(85, 300); // Give Golden Tommy Gun (smg_trench_a_v2)
                    pActivePlayer->GetInventoryWrapper()->AddWeapon(3, 200);  // Give Gold Pistol (p_mast_a_v2)

                    if (auto wepCtrl = pActivePlayer->GetHumanWeaponController()) {
                        wepCtrl->DoWeaponSelectByItemId(85, true); // probably not the best way to do it (double hands guns are handled with one)
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button("Throw weapon")) {
                    // Not sync
                    pActivePlayer->ThrowWeapon();
                }
            }

            if (ImGui::Button("Teleport to Salieri's Bar")) {
                SDK::ue::C_CntPtr<uintptr_t> syncObject;
                auto pos = SDK::ue::sys::math::C_Vector(-916.0, -210.0, 2.605);
                auto dir = SDK::ue::sys::math::C_Vector(1.0, 0.0, 0.0);
                SDK::GetPlayerTeleportModule()->TeleportPlayer(syncObject, pos, dir, false, false, false, false);
            }

            ImGui::Text("Player Ptr: %p", pActivePlayer);
        };

        CreateUIWindow("Player debug", windowContent, &_open);
    }
}; // namespace MafiaMP::Core::UI::Devs
