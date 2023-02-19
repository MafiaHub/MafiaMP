#pragma once

#include <stdint.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hooking_patterns.h>

namespace SDK {
    class Patterns {
      public:
        uint64_t C_Entity__GameInitAddr   = 0x0;
        uint64_t C_Entity__GameDoneAddr   = 0x0;
        uint64_t C_Entity__ActivateAddr   = 0x0;
        uint64_t C_Entity__DeactivateAddr = 0x0;
        uint64_t C_Entity__ReleaseAddr    = 0x0;

        uint64_t C_Game__GetGame = 0x0;

        uint64_t C_GameInputModule__GetGameInputModule = 0x0;
        uint64_t C_GameInputModule__PauseInput         = 0x0;

        uint64_t C_EntityList__GetEntityList = 0x0;

        uint64_t C_PlayerModelManager__IsPlayerLoaded          = 0x0;
        uint64_t C_PlayerModelManager__SwitchPlayerProfileAddr = 0x0;

        uint64_t C_ActorsSlotWrapper__C_ActorsSlotWrapperAddr  = 0x0;
        uint64_t C_ActorsSlotWrapper__UpdateToCreateActorsAddr = 0x0;
        uint64_t C_ActorsSlotWrapper__GetFreeActorAddr         = 0x0;
        uint64_t C_ActorsSlotWrapper__ReturnActorAddr          = 0x0;
        uint64_t C_ActorsSlotWrapper__CloseAddr                = 0x0;

        uint64_t C_SlotWrapper__LoadDataAddr = 0x0;

        uint64_t C_StreamingModule__SetStreamingPosSourceAddr = 0x0;

        uint64_t C_HumanSpawner__C_HumanSpawnerVtblAddr          = 0x0;
        uint64_t C_HumanSpawner__SetupDefaultArchetypeObjectAddr = 0x0;

        uint64_t C_ProfileSpawner__C_ProfileSpawnerAddr      = 0x0;
        uint64_t C_ProfileSpawner__C_ProfileSpawnerDctorAddr = 0x0;
        uint64_t C_ProfileSpawner__IsSpawnProfileLoadedAddr  = 0x0;
        uint64_t C_ProfileSpawner__CreateActorAddr           = 0x0;
        uint64_t C_ProfileSpawner__ReturnObjectAddr          = 0x0;

        uint64_t C_GameCamera__GetInstanceInternalAddr = 0x0;

        uint64_t C_WeatherManager2__GetDayTimeHoursAddr = 0x0;
        uint64_t C_WeatherManager2__GetDayTimeRelAddr   = 0x0;
        uint64_t C_WeatherManager2__SetDayTimeHoursAddr = 0x0;
        uint64_t C_WeatherManager2__SetDayTimeRelAddr   = 0x0;
        uint64_t C_WeatherManager2__SetWeatherSetAddr   = 0x0;

        uint64_t I_Core__GetInstance              = 0x0;
        uint64_t C_MafiaFramework__Instance       = 0x0;
        uint64_t C_GfxEnvironmentEffects_Instance = 0x0;
        uint64_t C_GameGUI2Module_Instance        = 0x0;

        uint64_t C_SceneObject__SetTransformAddr = 0x0;

        uint64_t renameme__SpawnObject  = 0x0;
        uint64_t renameme__SpawnObject2 = 0x0;
        uint64_t renameme__SpawnObject3 = 0x0;

        uint64_t C_Matrix__SetDir2Addr     = 0x0;
        uint64_t C_Matrix__SetDirAddr      = 0x0;
        uint64_t C_Matrix__SetDir3Addr     = 0x0;
        uint64_t C_Matrix__SetRotAddr      = 0x0;
        uint64_t C_Matrix__SetRotEulerAddr = 0x0;

        uint64_t C_HashName__ComputeHashAddr = 0x0;
        uint64_t C_HashName__SetNameAddr     = 0x0;

        uint64_t C_String__SetStringAddr = 0x0;

        uint64_t C_IE__AllocAddr = 0x0;
        uint64_t C_IE__FreeAddr  = 0x0;

        uint64_t C_TickedModuleManager__GetTickedModuleManager = 0x0;

        uint64_t C_Ctx__BeginUpdateAddr = 0x0;
        uint64_t C_Ctx__EndUpdateAddr   = 0x0;

        uint64_t I_VirtualFileSystemCache__GetInstance = 0x0;

        uint64_t C_CharacterController__ActivateHandler                           = 0x0;
        uint64_t C_CharacterController__DeactivateHandler_FromPlayerInput         = 0x0;
        uint64_t C_CharacterStateHandlerBaseLocomotion__Idle2MoveTransitionActive = 0x0;
        uint64_t C_CharacterStateHandlerBaseLocomotion__AddRemoveSprintDescriptor = 0x0;
        uint64_t C_CharacterStateHandlerMove__SharpTurnTransitionActive           = 0x0;
        uint64_t C_WAnimPlaybackManager__PlayState                                = 0x0;
        uint64_t C_BehaviorCharacter__SetWAnimVariable_float                      = 0x0;
        uint64_t C_CharacterController__TriggerActorActionById                    = 0x0;

        // C_HumanScript
        uint64_t C_HumanScript__GetOnVehicle  = 0x0;
        uint64_t C_HumanScript__GetOffVehicle = 0x0;
        uint64_t C_HumanScript__SetHealth     = 0x0;

        // C_Quat
        uint64_t C_Quat__SetDir = 0x0;

        // C_Car
        uint64_t C_Car__Lock   = 0x0;
        uint64_t C_Car__Unlock = 0x0;

        uint64_t C_Car__LockEntryPoints   = 0x0;
        uint64_t C_Car__UnlockEntryPoints = 0x0;

        uint64_t C_Car__OpenHood  = 0x0;
        uint64_t C_Car__CloseHood = 0x0;

        uint64_t C_Car__OpenTrunk  = 0x0;
        uint64_t C_Car__CloseTrunk = 0x0;

        uint64_t C_Car__SetMotorDamage = 0x0;
        uint64_t C_Car__GetMotorDamage = 0x0;

        uint64_t C_Car__GetDamage         = 0x0;
        uint64_t C_Car__SetTransparency   = 0x0;
        uint64_t C_Car__SetSpeed          = 0x0;
        uint64_t C_Car__PosefujZimuVShopu = 0x0;
        uint64_t C_Car__Restore           = 0x0;

        // C_Motor
        uint64_t C_Motor__SetFuel = 0x0;

        // C_Vehicle
        uint64_t C_Vehicle__AddVehicleFlags    = 0x0;
        uint64_t C_Vehicle__ClearVehicleFlags  = 0x0;
        uint64_t C_Vehicle__SetVehicleMatrix   = 0x0;
        uint64_t C_Vehicle__SetSPZText         = 0x0;
        uint64_t C_Vehicle__GetSPZText         = 0x0;
        uint64_t C_Vehicle__SetActive          = 0x0;
        uint64_t C_Vehicle__Damage             = 0x0;
        uint64_t C_Vehicle__SetBeaconLightsOn  = 0x0;
        uint64_t C_Vehicle__EnableRadio        = 0x0;
        uint64_t C_Vehicle__SetBrake           = 0x0;
        uint64_t C_Vehicle__SetEngineOn        = 0x0;
        uint64_t C_Vehicle__SetGear            = 0x0;
        uint64_t C_Vehicle__SetHandbrake       = 0x0;
        uint64_t C_Vehicle__SetPower           = 0x0;
        uint64_t C_Vehicle__SetSearchLightsOn  = 0x0;
        uint64_t C_Vehicle__SetSiren           = 0x0;
        uint64_t C_Vehicle__SetSpeedLimit      = 0x0;
        uint64_t C_Vehicle__SetSteer           = 0x0;
        uint64_t C_Vehicle__SetHorn            = 0x0;
        uint64_t C_Vehicle__TurnRadioOn        = 0x0;
        uint64_t C_Vehicle__DamageBreaks       = 0x0;
        uint64_t C_Vehicle__SetDoorFree        = 0x0;
        uint64_t C_Vehicle__SetSpeed           = 0x0;
        uint64_t C_Vehicle__SetAngularSpeed    = 0x0;
        uint64_t C_Vehicle__IsActive           = 0x0;
        uint64_t C_Vehicle__IsSiren            = 0x0;
        uint64_t C_Vehicle__SetVehicleDirty    = 0x0;
        uint64_t C_Vehicle__SetVehicleRust     = 0x0;
        uint64_t C_Vehicle__SetVehicleColor    = 0x0;
        uint64_t C_Vehicle__SetInteriorColors  = 0x0;
        uint64_t C_Vehicle__SetWindowTintColor = 0x0;
        uint64_t C_Vehicle__SetWheelTintColor  = 0x0;

        // C_Door
        uint64_t C_Door__Lock          = 0x0;
        uint64_t C_Door__AILock        = 0x0;
        uint64_t C_Door__Unlock        = 0x0;
        uint64_t C_Door__AIUnlock      = 0x0;
        uint64_t C_Door__StartLockpick = 0x0;
        uint64_t C_Door__StopLockpick  = 0x0;
        uint64_t C_Door__Kick          = 0x0;
        uint64_t C_Door__Open          = 0x0;
        uint64_t C_Door__Close         = 0x0;
        uint64_t C_Door__ToggleOpen    = 0x0;
        uint64_t C_Door__EnableAction  = 0x0;
        uint64_t C_Door__DisableAction = 0x0;

        // C_Human2CarWrapper
        uint64_t C_Human2CarWrapper__GetSeatID = 0x0;

        // C_InventoryWrapper
        uint64_t C_InventoryWrapper__AddMoney  = 0x0;
        uint64_t C_InventoryWrapper__AddWeapon = 0x0;
        uint64_t C_InventoryWrapper__TellMoney = 0x0;

        // C_HumanInventory
        uint64_t C_HumanInventory__AddItem                       = 0x0;
        uint64_t C_HumanInventory__AddItemByData                 = 0x0;
        uint64_t C_HumanInventory__AddWeapon                     = 0x0;
        uint64_t C_HumanInventory__AddMedkits                    = 0x0;
        uint64_t C_HumanInventory__CanAddAmmoByCategoryAux       = 0x0;
        uint64_t C_HumanInventory__CanAddAmmoByCategoryAuxAndMag = 0x0;
        uint64_t C_HumanInventory__CanAddUpgrade                 = 0x0;
        uint64_t C_HumanInventory__CanFire                       = 0x0;
        uint64_t C_HumanInventory__CanReload                     = 0x0;
        uint64_t C_HumanInventory__CanAddWeapon                  = 0x0;
        uint64_t C_HumanInventory__CanUseMedkit                  = 0x0;
        uint64_t C_HumanInventory__CreateDefaultItems            = 0x0;
        uint64_t C_HumanInventory__CreateModel                   = 0x0;
        uint64_t C_HumanInventory__DestroyModel                  = 0x0;
        uint64_t C_HumanInventory__DoReload                      = 0x0;
        uint64_t C_HumanInventory__DoShot                        = 0x0;
        uint64_t C_HumanInventory__DuplicateWeaponModel          = 0x0;
        uint64_t C_HumanInventory__GetSelectedAmmoCategory       = 0x0;
        uint64_t C_HumanInventory__SelectAnimSetting             = 0x0;
        uint64_t C_HumanInventory__SelectByItemID                = 0x0;
        uint64_t C_HumanInventory__SelectNextWeapon              = 0x0;
        uint64_t C_HumanInventory__TellMedkit                    = 0x0;
        uint64_t C_HumanInventory__UseMedkit                     = 0x0;

        // C_HumanWeaponController
        uint64_t C_HumanWeaponController__DoWeaponSelectByItemId = 0x0;
        uint64_t C_HumanWeaponController__GetRightHandWeaponID   = 0x0;
        uint64_t C_HumanWeaponController__GetShotPosDir          = 0x0;
        uint64_t C_HumanWeaponController__IsThrownWeapon         = 0x0;
        uint64_t C_HumanWeaponController__ResetScatterCoef       = 0x0;
        uint64_t C_HumanWeaponController__SetAiming              = 0x0;
        uint64_t C_HumanWeaponController__SetCoverFlag           = 0x0;
        uint64_t C_HumanWeaponController__SetFirePressedFlag     = 0x0;
        uint64_t C_HumanWeaponController__SetStickMove           = 0x0;
        uint64_t C_HumanWeaponController__SetZoomFlag            = 0x0;
        uint64_t C_HumanWeaponController__DoShot                 = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponReloadDropMagazine = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponReloadShowMagazine = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponReloadInventory = 0x0;

        uint64_t C_Human2__EnableShadows = 0x0;
        uint64_t C_Human2__EnableHumanClothes = 0x0;

        // C_Navigation
        uint64_t C_Navigation__GetInstance           = 0x0;
        uint64_t C_Navigation__SetUserMark           = 0x0;
        uint64_t C_Navigation__RegisterVehicleCommon = 0x0;
        uint64_t C_Navigation__RegisterVehicleEntity = 0x0;
        uint64_t C_Navigation__UnregisterVehicle     = 0x0;

        // init
        uint64_t C_InitDone_MafiaFrameworkAddr = 0x0;
        uint64_t LoadIntroAddr                 = 0x0;

        // C_CommandLine
        uint64_t C_CommandLine__FindCommandAddr = 0x0;

        // C_GameFramework
        uint64_t C_GameFramework__IsSuspendedAddr = 0x0;

        // Lua
        uint64_t Lua__pcallAddr      = 0x0;
        uint64_t Lua__loadbufferAddr = 0x0;
        uint64_t Lua__tostringAddr   = 0x0;
        uint64_t Lua__isstringAddr   = 0x0;

        static void InitPatterns();
    };

    extern Patterns gPatterns;
} // namespace SDK
