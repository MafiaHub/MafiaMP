#pragma once

#include <cstdint>

namespace SDK {
    class I_Human2;

    namespace mafia::ui::hud {
        class I_HudController;
    }

    class C_GuiController {
      public:
        enum class E_AimTarget : int32_t {
            E_AT_0         = 0,
            E_AT_1         = 1,
            E_AT_2         = 2,
            E_AT_AIM_BLOCKED = 3,
            E_AT_DEFAULT   = 4,
        };

        // Reversed from:
        // - Constructor: C_GuiController::C_GuiController (0x142baa210)
        // - UpdateReticles: C_GuiController::UpdateReticles (0x142bd6080)

        C_GuiController(I_Human2 *human);

        void Update(float dt, bool a2, bool a3, bool a4, bool a5, bool a6, bool a7, float a8, E_AimTarget aimTarget);
        void UpdateCarHud(bool a1);
        void UpdatePlayer();
        void UpdatePlayerExplosiveCounts();
        void UpdateReticles(E_AimTarget aimTarget, bool a2, bool a3, bool a4, bool a5);

        void CreateCrosshairVisibilityStructs(mafia::ui::hud::I_HudController *hudController);

        void SetIsInCar(bool inCar);
        void SetZoom(bool zoom);
        bool DisabledByControlStyleHint();

        // Perk menu
        void OpenPerkMenu();
        void ClosePerkMenu();
        int32_t GetCurrentlySelectedPerkItem();
        void IncrementPerkSelection(int32_t direction);

        // Inventory
        void InventoryAdd(uint32_t weaponId);
        void InventoryClear();

        // Weapon
        void OnReloadWeapon();
        void RefreshEquippedWeaponAmmo();

        I_Human2 *GetHuman() const {
            return m_pHuman;
        }

        int32_t m_iUnk00;                // 0x00 - 0x04 (init: 0)
        bool m_bUnk04;                   // 0x04 - 0x05 (init: false)
        char _pad05[0x3];                // 0x05 - 0x08
        int32_t m_iUnk08;               // 0x08 - 0x0C (init: -1)
        int32_t m_iUnk0C;               // 0x0C - 0x10 (init: -1)
        int32_t m_iCrosshairCommandId;   // 0x10 - 0x14 (init: -1, crosshair command handle)
        char _pad14[0x4];               // 0x14 - 0x18
        void *m_pCrosshairVis0;          // 0x18 - 0x20 (ref-counted crosshair visibility struct)
        void *m_pCrosshairVis1;          // 0x20 - 0x28 (ref-counted crosshair visibility struct)
        void *m_pCrosshairVis2;          // 0x28 - 0x30 (ref-counted crosshair visibility struct)
        void *m_pUnk30;                  // 0x30 - 0x38 (init: nullptr)
        I_Human2 *m_pHuman;             // 0x38 - 0x40 (owner human)
    };

    static_assert(sizeof(C_GuiController) == 0x40, "C_GuiController size mismatch");
    static_assert(offsetof(C_GuiController, m_pHuman) == 0x38, "C_GuiController::m_pHuman offset mismatch");
} // namespace SDK
