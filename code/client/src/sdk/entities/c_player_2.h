#pragma once

#include "c_human_2.h"
#include "sdk/mafia/health_system/c_health_system.h"
#include "sdk/ue/sys/math/c_vector.h"

namespace SDK {
    namespace mafia {
        namespace framework {
            class C_Blacklist;
        }
        namespace playereffects {
            class C_PlayerEffects;
        }
    }
    namespace ue::game::script {
        class C_ScriptRangedScalarController;
    }

    class C_GuiController;
    class C_Perks;

    class C_Player2 : public C_Human2 {
      public:
        char _pad04B0[0x18];                                             // 0x4B0 - 0x4C8
        void *m_pUnk04C8;                                                // 0x4C8 - 0x4D0
        bool m_bUnk04D0;                                                 // 0x4D0 - 0x4D1
        char _pad04D1[0x3];                                              // 0x4D1 - 0x4D4
        ue::sys::math::C_Vector2 m_vMovementInput;                       // 0x4D4 - 0x4DC
        ue::sys::math::C_Vector2 m_vCameraInput;                         // 0x4DC - 0x4E4
        ue::sys::math::C_Vector2 m_vUnkVector04E4;                       // 0x4E4 - 0x4EC
        char _pad04EC[0x104];                                            // 0x4EC - 0x5F0 (includes sub-object at 0x4EC)
        char m_aSlotData[0x3A0];                                         // 0x5F0 - 0x990 (29 * 32 bytes, slot array)
        char m_aUnkArray[0x2B8];                                         // 0x990 - 0xC48 (29 * 24 bytes)
        float m_fUnk0C48;                                                // 0xC48 - 0xC4C (init: -1.0f)
        char _pad0C4C[0x4];                                              // 0xC4C - 0xC50
        void *m_pUnk0C50;                                                // 0xC50 - 0xC58
        float m_fUnk0C58;                                                // 0xC58 - 0xC5C (init: -1.0f)
        char _pad0C5C[0x4];                                              // 0xC5C - 0xC60
        void *m_pUnk0C60;                                                // 0xC60 - 0xC68
        void *m_pUnk0C68;                                                // 0xC68 - 0xC70
        void *m_pUnk0C70;                                                // 0xC70 - 0xC78
        float m_fUnk0C78;                                                // 0xC78 - 0xC7C (init: 2.0f)
        float m_fUnk0C7C;                                                // 0xC7C - 0xC80 (init: 0.2f)
        float m_fUnk0C80;                                                // 0xC80 - 0xC84 (init: 0.2f)
        char _pad0C84[0x4];                                              // 0xC84 - 0xC88
        C_GuiController *m_pGuiController;                               // 0xC88 - 0xC90
        void *m_pUnk0C90;                                                // 0xC90 - 0xC98
        void *m_pUnk0C98;                                                // 0xC98 - 0xCA0
        bool m_bUnk0CA0;                                                 // 0xCA0 - 0xCA1
        char _pad0CA1[0x3];                                              // 0xCA1 - 0xCA4
        int32_t m_iUnk0CA4;                                              // 0xCA4 - 0xCA8 (init: 8)
        int32_t m_iUnk0CA8;                                              // 0xCA8 - 0xCAC (init: 3)
        bool m_bUnk0CAC;                                                 // 0xCAC - 0xCAD
        char _pad0CAD[0x3];                                              // 0xCAD - 0xCB0
        void *m_pUnk0CB0;                                                // 0xCB0 - 0xCB8
        void *m_pUnk0CB8;                                                // 0xCB8 - 0xCC0
        char _pad0CC0[0x18];                                             // 0xCC0 - 0xCD8
        int32_t m_iUnk0CD8;                                              // 0xCD8 - 0xCDC (init: 2)
        float m_fUnk0CDC;                                                // 0xCDC - 0xCE0 (init: 6.0f)
        float m_fUnk0CE0;                                                // 0xCE0 - 0xCE4
        int16_t m_iUnk0CE4;                                              // 0xCE4 - 0xCE6 (init: 1)
        bool m_bUnk0CE6;                                                 // 0xCE6 - 0xCE7
        char _pad0CE7[0x1];                                              // 0xCE7 - 0xCE8
        void *m_pUnk0CE8;                                                // 0xCE8 - 0xCF0
        float m_fUnk0CF0;                                                // 0xCF0 - 0xCF4 (init: ~0.44f)
        float m_fUnk0CF4;                                                // 0xCF4 - 0xCF8 (init: 6.0f)
        void *m_pUnk0CF8;                                                // 0xCF8 - 0xD00
        bool m_bUnk0D00;                                                 // 0xD00 - 0xD01
        bool m_bUnk0D01;                                                 // 0xD01 - 0xD02
        char _pad0D02[0x2E];                                             // 0xD02 - 0xD30
        mafia::health_system::C_HealthSystem *m_pPlayerHealthSystem;     // 0xD30 - 0xD38
        void *m_pUnk0D38;                                                // 0xD38 - 0xD40
        uint32_t m_uFlags0D40;                                           // 0xD40 - 0xD44 (bit flags)
        char _pad0D44[0x34];                                             // 0xD44 - 0xD78
        void *m_pUnk0D78;                                                // 0xD78 - 0xD80
        void *m_pSharpshooterEventDispatcher;                            // 0xD80 - 0xD88
        mafia::playereffects::C_PlayerEffects *m_pPlayerEffects;         // 0xD88 - 0xD90
        void *m_pUnk0D90;                                                // 0xD90 - 0xD98 (80-byte object)
        void *m_pUnk0D98;                                                // 0xD98 - 0xDA0
        C_Perks *m_pPerks;                                               // 0xDA0 - 0xDA8
        void *m_pUnk0DA8;                                                // 0xDA8 - 0xDB0
        int16_t m_iUnk0DB0;                                              // 0xDB0 - 0xDB2
        char _pad0DB2[0x6];                                              // 0xDB2 - 0xDB8
        mafia::framework::C_Blacklist *m_pBlacklist;                     // 0xDB8 - 0xDC0
        void *m_pUnk0DC0;                                                // 0xDC0 - 0xDC8 (32-byte object)
        void *m_pUnk0DC8;                                                // 0xDC8 - 0xDD0 (72-byte object)
        void *m_pUnk0DD0;                                                // 0xDD0 - 0xDD8 (80-byte controller)
        ue::game::script::C_ScriptRangedScalarController *m_pScriptRangedScalarController; // 0xDD8 - 0xDE0
        void *m_pTimerController;                                        // 0xDE0 - 0xDE8 (128-byte object with C_SimpleTimer)
        int32_t m_iUnk0DE8;                                              // 0xDE8 - 0xDEC
        uint8_t m_uUnk0DEC;                                              // 0xDEC - 0xDED (init: 1)
        uint8_t m_uUnk0DED;                                              // 0xDED - 0xDEE (init: 1)
        bool m_bUnk0DEE;                                                 // 0xDEE - 0xDEF (init: true)
        char _pad0DEF[0x1];                                              // 0xDEF - 0xDF0
        void *m_pUnk0DF0;                                                // 0xDF0 - 0xDF8
        void *m_pUnk0DF8;                                                // 0xDF8 - 0xE00
        int16_t m_iUnk0E00;                                              // 0xE00 - 0xE02
        char _pad0E02[0x6];                                              // 0xE02 - 0xE08
        void *m_pDeathMarkerEntity;                                      // 0xE08 - 0xE10
        bool m_bUnk0E10;                                                 // 0xE10 - 0xE11
        char _pad0E11[0x1F];                                             // 0xE11 - 0xE30
        void *m_pUnk0E30;                                                // 0xE30 - 0xE38
        void *m_pUnk0E38;                                                // 0xE38 - 0xE40
        int32_t m_iUnk0E40;                                              // 0xE40 - 0xE44 (init: -1)
        char _pad0E44[0x4];                                              // 0xE44 - 0xE48
        void *m_pUnk0E48;                                                // 0xE48 - 0xE50
        void *m_pUnk0E50;                                                // 0xE50 - 0xE58
        void *m_pUnk0E58;                                                // 0xE58 - 0xE60
        void *m_pUnk0E60;                                                // 0xE60 - 0xE68

      public:
        virtual bool UsingMountedWeapon() = 0;

      public:
        C_GuiController *GetGuiController() const {
            return m_pGuiController;
        }

        C_Perks *GetPerks() const {
            return m_pPerks;
        }

        mafia::playereffects::C_PlayerEffects *GetPlayerEffects() const {
            return m_pPlayerEffects;
        }

        mafia::framework::C_Blacklist *GetBlacklist() const {
            return m_pBlacklist;
        }

        mafia::health_system::C_HealthSystem *GetPlayerHealthSystem() const {
            return m_pPlayerHealthSystem;
        }

        mafia::health_system::C_HealthBar *GetPlayerHealthBar() const {
            if (m_pPlayerHealthSystem) {
                return m_pPlayerHealthSystem->GetHealthBar();
            }
            return nullptr;
        }

        float GetPlayerHealth() const {
            if (m_pPlayerHealthSystem) {
                return m_pPlayerHealthSystem->GetCurrentHealth();
            }
            return m_fHealth;
        }

        float GetPlayerMaxHealth() const {
            if (m_pPlayerHealthSystem) {
                return m_pPlayerHealthSystem->GetMaxHealth();
            }
            return m_fHealthMax;
        }

        bool IsInDangerZone() const {
            if (m_pPlayerHealthSystem) {
                return m_pPlayerHealthSystem->IsInDangerZone();
            }
            return false;
        }
    };

    static_assert(offsetof(C_Player2, m_pGuiController) == 0xC88, "C_Player2::m_pGuiController offset mismatch");
    static_assert(offsetof(C_Player2, m_pPlayerHealthSystem) == 0xD30, "C_Player2::m_pPlayerHealthSystem offset mismatch");
    static_assert(offsetof(C_Player2, m_pSharpshooterEventDispatcher) == 0xD80, "C_Player2::m_pSharpshooterEventDispatcher offset mismatch");
    static_assert(offsetof(C_Player2, m_pPlayerEffects) == 0xD88, "C_Player2::m_pPlayerEffects offset mismatch");
    static_assert(offsetof(C_Player2, m_pPerks) == 0xDA0, "C_Player2::m_pPerks offset mismatch");
    static_assert(offsetof(C_Player2, m_pBlacklist) == 0xDB8, "C_Player2::m_pBlacklist offset mismatch");
    static_assert(offsetof(C_Player2, m_pScriptRangedScalarController) == 0xDD8, "C_Player2::m_pScriptRangedScalarController offset mismatch");
    static_assert(offsetof(C_Player2, m_pDeathMarkerEntity) == 0xE08, "C_Player2::m_pDeathMarkerEntity offset mismatch");

} // namespace SDK
