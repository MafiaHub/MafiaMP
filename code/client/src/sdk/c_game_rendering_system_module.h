#pragma once

#include "c_ticked_module.h"
#include "ue/c_application_win32.h"

#include "patterns.h"

#include <cstdint>
#include <vector>

namespace SDK {
    namespace ue::sys::core {
        class C_RenderDestination;
    }

    struct S_ResolutionEntry {
        uint32_t m_nWidth;       // 0x00 - 0x04
        uint32_t m_nHeight;      // 0x04 - 0x08
        uint32_t m_nUnk08;      // 0x08 - 0x0C
        uint32_t m_nUnk0C;      // 0x0C - 0x10
        uint8_t m_nUnk10;       // 0x10 - 0x11
        char _pad11[0x3];        // 0x11 - 0x14
    };
    static_assert(sizeof(S_ResolutionEntry) == 0x14, "S_ResolutionEntry size mismatch");

    class C_GameRenderingSystemModule : public C_TickedModule {
      public:
        uint32_t m_nVideoResolutionIndex;                              // 0x08 - 0x0C
        char _pad0C[0x4];                                              // 0x0C - 0x10
        std::vector<S_ResolutionEntry> m_vResolutions;                 // 0x10 - 0x28
        uint32_t m_nWindowBorderWidth;                                 // 0x28 - 0x2C
        uint32_t m_nWindowBorderHeight;                                // 0x2C - 0x30
        bool m_bFastRenderMode;                                        // 0x30 - 0x31
        char _pad31[0x3];                                              // 0x31 - 0x34
        uint32_t m_nPresentationInterval;                              // 0x34 - 0x38
        bool m_bUnk38;                                                 // 0x38 - 0x39
        bool m_bUnk39;                                                 // 0x39 - 0x3A
        char _pad3A[0x2];                                              // 0x3A - 0x3C
        float m_fGammaOverride;                                        // 0x3C - 0x40 (init: -1.0f)
        uint16_t m_nUnk40;                                             // 0x40 - 0x42
        char _pad42[0x6];                                              // 0x42 - 0x48
        ue::sys::core::C_RenderDestination *m_pRenderDestination;      // 0x48 - 0x50
        void *m_pSceneRenderContext;                                   // 0x50 - 0x58
        void *m_pTreeSentinel;                                         // 0x58 - 0x60
        void *m_pUnk60;                                                // 0x60 - 0x68
        char m_saveBlock[0x28];                                        // 0x68 - 0x90 (C_SaveBlock)
        float m_fGameplayFOVMultiplier;                                // 0x90 - 0x94
        bool m_bGameplayDepthOfFieldEnabled;                           // 0x94 - 0x95
        char _pad95[0x3];                                              // 0x95 - 0x98
        uint32_t m_nGlobalSettingsPreset;                              // 0x98 - 0x9C
        float m_fGammaCorrection;                                      // 0x9C - 0xA0
        uint32_t m_nAmbientOcclusion;                                  // 0xA0 - 0xA4 (E_GraphicQuality)
        uint32_t m_nGeometryDetail;                                    // 0xA4 - 0xA8 (E_GraphicQuality)
        uint32_t m_nReflectionQuality;                                 // 0xA8 - 0xAC (E_GraphicQuality)
        uint32_t m_nVolumetricEffectsQuality;                          // 0xAC - 0xB0 (E_GraphicQuality)
        uint32_t m_nUnkB0;                                             // 0xB0 - 0xB4
        uint32_t m_nMotionBlurEnabled;                                 // 0xB4 - 0xB8
        uint32_t m_nAntiAliasing;                                      // 0xB8 - 0xBC (E_GraphicQuality)
        uint32_t m_nShadowQuality;                                     // 0xBC - 0xC0 (E_GraphicQuality)
        uint32_t m_nUnkC0;                                             // 0xC0 - 0xC4
        uint32_t m_nFPS;                                               // 0xC4 - 0xC8
        uint32_t m_nVideoResolutionMode;                               // 0xC8 - 0xCC
        uint32_t m_nResolutionWidth;                                   // 0xCC - 0xD0
        uint32_t m_nResolutionHeight;                                  // 0xD0 - 0xD4
        bool m_bVerticalSync;                                          // 0xD4 - 0xD5
        bool m_bVideoSettingsInitialized;                              // 0xD5 - 0xD6
        bool m_bFullscreen;                                            // 0xD6 - 0xD7
        bool m_bWindowBorder;                                          // 0xD7 - 0xD8
        char _padD8[0x8];                                              // 0xD8 - 0xE0
        void *m_pSubObjVtable;                                         // 0xE0 - 0xE8
        void *m_pUnkE8;                                                // 0xE8 - 0xF0
        void *m_pUnkF0;                                                // 0xF0 - 0xF8
        uint32_t m_nUnkF8;                                             // 0xF8 - 0xFC
        char _padFC[0x4];                                              // 0xFC - 0x100
        ue::C_Application_Win32 *m_pApplication;                       // 0x100 - 0x108
        HWND m_hWnd;                                                   // 0x108 - 0x110

        virtual void SystemInit(I_TickedModuleCallEventContext &ctx);
        virtual bool GetFullscreen();
        virtual void Unk8();
        virtual bool GetVerticalSync();
        virtual void SetVerticalSync(bool bEnable);
        virtual bool GetWindowBorder();
        virtual void SetWindowBorder(bool bEnable);
        virtual bool GetGameplayDepthOfFieldEnabled() const;
        virtual void SetGameplayDepthOfFieldEnabled(bool bEnable);
        virtual float GetGameplayFOVMultiplier() const;
        virtual void SetGameplayFOVMultiplier(float fMultiplier);
        virtual void SetMotionBlurEnabled_Virtual(bool bEnable);
        virtual void SetGammaCorrection_Virtual(float fGamma);
        virtual bool GetGraphicsSettingsPreset(uint32_t quality, void *pOutSettings);
        virtual void SetAntiAliasing(uint32_t quality);
        virtual void SetShadowQuality(uint32_t quality);
        virtual void Unk22(uint32_t quality);
        virtual void SetGeometryDetail(uint32_t quality);
        virtual void SetReflectionQuality(uint32_t quality);
        virtual void SetVolumetricEffectsQuality(uint32_t quality);
        virtual void SetAmbientOcclusion(uint32_t quality);
        virtual void Unk27(uint32_t quality);
        virtual void SetSceneRenderEnabled(bool bEnable);
        virtual void Unk29();
        virtual void ResetAllVideoSettingsToDefault();
        virtual void Unk31();
        virtual void DetectSupportedResolutions();
        virtual void SetFastRenderMode_Virtual(bool bEnable);
        virtual void Unk34();
        virtual void *CreateRenderDestination(int renderingSystemID, uint32_t width, uint32_t height);

        static C_GameRenderingSystemModule *GetGameRenderingSystemModule() {
            return hook::call<C_GameRenderingSystemModule *>(gPatterns.C_GameRenderingSystemModule__GetGameRenderingSystemModule);
        }
    };
    static_assert(sizeof(C_GameRenderingSystemModule) == 0x110, "C_GameRenderingSystemModule size mismatch");
} // namespace SDK
