#pragma once

#include "c_ticked_module.h"
#include "c_ticked_module_manager.h"

#include "sdk/ue/game/input/c_game_input.h"
#include "sdk/ue/sys/input/c_sys_input.h"

#include <cstdint>
#include <vector>

namespace SDK {
    namespace ue::game::input {
        class C_InputSet;
        class C_InputLayer;
    } // namespace ue::game::input

    namespace ue::sys::input {
        class C_Device;
        class C_Joystick;
    } // namespace ue::sys::input

    enum class E_ControllerType : uint32_t {
        KeyboardMouse = 0,
        Gamepad       = 1,
        SteeringWheel = 2
    };

    enum class E_Controls : uint32_t {
        StartButton = 16
        // 157 total control bindings
    };

    // Size: 0x30 (48 bytes)
    struct S_ControlBinding {
        uint64_t pad0;           // 0x00 - 0x08
        float previousValue;     // 0x08 - 0x0C
        int32_t flags;           // 0x0C - 0x10
        float currentValue;      // 0x10 - 0x14
        int32_t unknown1_a;      // 0x14 - 0x18
        int32_t unknown1_b;      // 0x18 - 0x1C
        int32_t unknown2;        // 0x1C - 0x20
        uint64_t unknown3;       // 0x20 - 0x28
        int32_t unknown4;        // 0x28 - 0x2C
        char pad1[4];            // 0x2C - 0x30
    };
    static_assert(sizeof(S_ControlBinding) == 0x30, "S_ControlBinding size mismatch");

    class I_PrimaryDeviceListener {
      public:
        virtual ~I_PrimaryDeviceListener()                                       = default;
        virtual void OnStartButtonPressed(ue::sys::input::C_Joystick *pJoystick) = 0;
        virtual void OnJoystickConnected()                                       = 0;
        virtual void OnPrimaryDeviceChanged()                                    = 0;
        virtual void OnControllerTypeChanged()                                   = 0;
    };

    // Size: 0x80 (128 bytes)
    // Reversed from C_GameInputModule::Init (0x142e2dc30)
    // Offset 0x48 (72) has QWORD for player binding
    // Offset 0x64 (100) has DWORD for controller type
    class C_GameInputSaveBlock {
      public:
        char pad0[0x28];                    // 0x00 - 0x28
        void *m_pSomeData1;                 // 0x28 - 0x30
        void *m_pSomeData2;                 // 0x30 - 0x38
        void *m_pSomeData3;                 // 0x38 - 0x40
        int32_t m_unknown1;                 // 0x40 - 0x44
        char pad1[0x04];                    // 0x44 - 0x48
        void *m_pPlayerBinding;             // 0x48 - 0x50
        void *m_pCarBinding;                // 0x50 - 0x58
        void *m_pUnknown;                   // 0x58 - 0x60
        int32_t m_unknown2;                 // 0x60 - 0x64
        int32_t m_playerControllerType;     // 0x64 - 0x68
        int32_t m_carControllerType;        // 0x68 - 0x6C
        float m_gammaValue1;                // 0x6C - 0x70
        float m_gammaValue2;                // 0x70 - 0x74
        float m_sensitivity;                // 0x74 - 0x78
        int32_t m_flags;                    // 0x78 - 0x7C
        bool m_usingController;             // 0x7C - 0x7D
        char pad2[3];                       // 0x7D - 0x80

      public:
        static void RegisterToFactory();
    };

    constexpr int NUM_CONTROL_BINDINGS = 157;

    /**
     * C_GameInputModule
     * Size: 0x59F8 (23032 bytes)
     */
    class C_GameInputModule : public C_TickedModule {
      public:
        void *m_pIEngineAwareVtable;                                              // 0x0008 - 0x0010
        bool m_bUnknown1;                                                         // 0x0010 - 0x0011
        char pad0[0x07];                                                          // 0x0011 - 0x0018
        ue::game::input::C_GameInput *m_pGameInput;                               // 0x0018 - 0x0020
        ue::sys::input::C_SysInput *m_pSysInput;                                  // 0x0020 - 0x0028
        void *m_pDeviceChangeListener;                                            // 0x0028 - 0x0030
        std::vector<ue::sys::input::C_Device *> m_playerDevices;                  // 0x0030 - 0x0048
        std::vector<ue::sys::input::C_Device *> m_carDevices;                     // 0x0048 - 0x0060
        char pad1[0x10];                                                          // 0x0060 - 0x0070
        S_ControlBinding m_keyboardMouseBindings[NUM_CONTROL_BINDINGS];           // 0x0070 - 0x1DE0
        S_ControlBinding m_gamepadBindings[NUM_CONTROL_BINDINGS];                 // 0x1DE0 - 0x3B50
        S_ControlBinding m_steeringWheelBindings[NUM_CONTROL_BINDINGS];           // 0x3B50 - 0x58C0
        char pad2[0x08];                                                          // 0x58C0 - 0x58C8
        float m_fSensitivity;                                                     // 0x58C8 - 0x58CC
        uint16_t m_inputFlags;                                                    // 0x58CC - 0x58CE
        uint16_t m_startPressedFlag;                                              // 0x58CE - 0x58D0
        uint16_t m_unknown2;                                                      // 0x58D0 - 0x58D2
        char pad3[0x06];                                                          // 0x58D2 - 0x58D8
        ue::game::input::C_InputSet *m_pInputSet;                                 // 0x58D8 - 0x58E0
        ue::game::input::C_InputLayer *m_pMenuInputLayer;                         // 0x58E0 - 0x58E8
        ue::game::input::C_InputLayer *m_pInputLayer2;                            // 0x58E8 - 0x58F0
        ue::game::input::C_InputLayer *m_pPlayerInputLayer;                       // 0x58F0 - 0x58F8
        ue::game::input::C_InputLayer *m_pInputLayer4;                            // 0x58F8 - 0x5900
        ue::game::input::C_InputLayer *m_pCarInputLayer;                          // 0x5900 - 0x5908
        bool m_bInitialized;                                                      // 0x5908 - 0x5909
        bool m_bSkipTick;                                                         // 0x5909 - 0x590A
        char pad4[0x06];                                                          // 0x590A - 0x5910
        std::vector<void *> m_vector1;                                            // 0x5910 - 0x5928
        std::vector<void *> m_vector2;                                            // 0x5928 - 0x5940
        std::vector<void *> m_vector3;                                            // 0x5940 - 0x5958
        std::vector<void *> m_vector4;                                            // 0x5958 - 0x5970
        bool m_bCanSwitchDevice;                                                  // 0x5970 - 0x5971
        char pad5[0x03];                                                          // 0x5971 - 0x5974
        int32_t m_unknown3;                                                       // 0x5974 - 0x5978
        void *m_pCurrentDevice1;                                                  // 0x5978 - 0x5980
        uint32_t m_dwUnk5980;                                                     // 0x5980 - 0x5984
        E_ControllerType m_currentControllerType;                                 // 0x5984 - 0x5988
        void *m_pCurrentKeyboardDevice;                                           // 0x5988 - 0x5990
        void *m_pCurrentMouseDevice;                                              // 0x5990 - 0x5998
        bool m_bUnknown5;                                                         // 0x5998 - 0x5999
        char pad6[0x07];                                                          // 0x5999 - 0x59A0
        C_GameInputSaveBlock *m_pSaveBlock;                                       // 0x59A0 - 0x59A8
        void *m_pISaveBlockClientVtable;                                          // 0x59A8 - 0x59B0
        void *m_pContext;                                                         // 0x59B0 - 0x59B8
        bool m_bStartButtonWatching;                                              // 0x59B8 - 0x59B9
        char pad7[0x03];                                                          // 0x59B9 - 0x59BC
        E_Controls m_startButtonControl;                                          // 0x59BC - 0x59C0
        ue::sys::input::C_Joystick *m_pJoystickForStart;                          // 0x59C0 - 0x59C8
        void *m_pPrimaryDeviceListenersRoot;                                      // 0x59C8 - 0x59D0
        void *m_pUnknown6;                                                        // 0x59D0 - 0x59D8
        void *m_pNotificationListener;                                            // 0x59D8 - 0x59E0
        void *m_pInputSetChangedListener;                                         // 0x59E0 - 0x59E8
        void *m_pInputBindingManager;                                             // 0x59E8 - 0x59F0
        bool m_bUnknown7;                                                         // 0x59F0 - 0x59F1
        char pad8[0x07];                                                          // 0x59F1 - 0x59F8

      public:
        virtual ~C_GameInputModule();
        virtual uint64_t GetClassTypeInfo() const;
        virtual uint64_t GetTypeInfo() const;
        virtual const char *GetTickedModuleName();
        virtual void StaticRegister();
        virtual float GetTimeBudget() const;

        void Init(I_TickedModuleCallEventContext &context);
        void Done(I_TickedModuleCallEventContext &context);
        void Tick(I_TickedModuleCallEventContext &context);
        void GameInit(I_TickedModuleCallEventContext &context);
        void GameQuit(I_TickedModuleCallEventContext &context);
        void OnSlotsUploading(I_TickedModuleCallEventContext &context);
        void OnGamePaused(I_TickedModuleCallEventContext &context);
        void OnGameUnPaused(I_TickedModuleCallEventContext &context);
        void ActivateApp(I_TickedModuleCallEventContext &context);
        void DeactivateApp(I_TickedModuleCallEventContext &context);
        void InitSteamController(I_TickedModuleCallEventContext &context);
        void RegisterKeyboradLightingCallback(I_TickedModuleCallEventContext &context);

        bool IsInitialized() const;
        void SetMouseExclusivity(bool exclusive);
        void AddPrimaryDeviceListener(I_PrimaryDeviceListener *pListener);
        void NotifyClientsAboutStartPressed(ue::sys::input::C_Joystick *pJoystick);
        void UpdatePlayerControlDeviceType();
        void UpdateCarControllerType();
        void SetNewControllers(E_ControllerType controllerType, const std::vector<ue::sys::input::C_Device *> &devices);
        void PlayRumbleEffect(uint32_t effectId, void *entity, uint8_t param, int64_t param2, int64_t param3);
        void StopAllRumbleEffects();
        void *FindPlayerControlForHint(E_Controls control) const;
        void PauseInput(bool pause);
    };
    static_assert(sizeof(C_GameInputModule) == 0x59F8, "C_GameInputModule size mismatch");

    C_GameInputModule *GetGameInputModule();
} // namespace SDK
