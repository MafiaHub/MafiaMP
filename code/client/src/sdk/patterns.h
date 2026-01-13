#pragma once

#include <stdint.h>
#include <utils/hooking/hooking.h>
#include <utils/hooking/hooking_patterns.h>

namespace SDK {
    class Patterns {
      public:
        // C_ActorsSlotWrapper
        uint64_t C_ActorsSlotWrapper__C_ActorsSlotWrapper  = 0x0;
        uint64_t C_ActorsSlotWrapper__Close                = 0x0;
        uint64_t C_ActorsSlotWrapper__GetFreeActor         = 0x0;
        uint64_t C_ActorsSlotWrapper__ReturnActor          = 0x0;
        uint64_t C_ActorsSlotWrapper__UpdateToCreateActors = 0x0;

        // C_BehaviorCharacter
        uint64_t C_BehaviorCharacter__SetWAnimVariable_float = 0x0;

        // C_Car
        uint64_t C_Car__CloseHood           = 0x0;
        uint64_t C_Car__CloseTrunk          = 0x0;
        uint64_t C_Car__ExplodeCar          = 0x0;
        uint64_t C_Car__ExplodeCar_2        = 0x0;
        uint64_t C_Car__GetDamage           = 0x0;
        uint64_t C_Car__GetFuelTankCapacity = 0x0;
        uint64_t C_Car__GetMotorDamage      = 0x0;
        uint64_t C_Car__Lock                = 0x0;
        uint64_t C_Car__LockEntryPoints     = 0x0;
        uint64_t C_Car__OpenHood            = 0x0;
        uint64_t C_Car__OpenTrunk           = 0x0;
        uint64_t C_Car__RestoreCar          = 0x0;
        uint64_t C_Car__SetActualFuel       = 0x0;
        uint64_t C_Car__SetMotorDamage      = 0x0;
        uint64_t C_Car__SetSeatStatus       = 0x0;
        uint64_t C_Car__SetSpeed            = 0x0;
        uint64_t C_Car__SetTransparency     = 0x0;
        uint64_t C_Car__SetVehicleDirty     = 0x0;
        uint64_t C_Car__Unlock              = 0x0;
        uint64_t C_Car__UnlockEntryPoints   = 0x0;

        // C_CharacterController
        uint64_t C_CharacterController__ActivateHandler                   = 0x0;
        uint64_t C_CharacterController__DeactivateHandler_FromPlayerInput = 0x0;
        uint64_t C_CharacterController__TriggerActorActionById            = 0x0;

        // C_CharacterStateHandlerAim
        uint64_t C_CharacterStateHandlerAim__Activate                                 = 0x0;
        uint64_t C_CharacterStateHandlerAim__Deactivate                               = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateRequest                            = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateAIFreq                             = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateHumanFreq                          = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateHumanFreqPostPhysics               = 0x0;
        uint64_t C_CharacterStateHandlerAim__Start                                    = 0x0;
        uint64_t C_CharacterStateHandlerAim__Finish                                   = 0x0;
        uint64_t C_CharacterStateHandlerAim__AcceptMessage                            = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdatePlayerInput                        = 0x0;
        uint64_t C_CharacterStateHandlerAim__OnCharacterControllerActivated           = 0x0;
        uint64_t C_CharacterStateHandlerAim__OnCharacterControllerDeactivated         = 0x0;
        uint64_t C_CharacterStateHandlerAim__OnStartBlindFire                         = 0x0;
        uint64_t C_CharacterStateHandlerAim__SwappingWeapon                           = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateAimAnimation                       = 0x0;
        uint64_t C_CharacterStateHandlerAim__IsAimAllowed                             = 0x0;
        uint64_t C_CharacterStateHandlerAim__IsAimBlocked                             = 0x0;
        uint64_t C_CharacterStateHandlerAim__IsReloadBlocked                          = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateAimDirection                       = 0x0;
        uint64_t C_CharacterStateHandlerAim__CalculatePlayerAimYawPitch               = 0x0;
        uint64_t C_CharacterStateHandlerAim__CalculatePlayerAimYawPitchFreeAndBlindFire = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdatePlayerAimBlocked                   = 0x0;
        uint64_t C_CharacterStateHandlerAim__UpdateHumanFreqAI                        = 0x0;

        // C_CharacterStateHandlerBaseLocomotion
        uint64_t C_CharacterStateHandlerBaseLocomotion__AddRemoveSprintDescriptor = 0x0;
        uint64_t C_CharacterStateHandlerBaseLocomotion__Idle2MoveTransitionActive = 0x0;

        // C_CharacterStateHandlerMove
        uint64_t C_CharacterStateHandlerMove__SharpTurnTransitionActive = 0x0;

        // C_CommandLine
        uint64_t C_CommandLine__FindCommand = 0x0;
        uint64_t C_CommandLine__GetCommandLineFileName = 0x0;
        uint64_t C_CommandLine__GetCommandLineFromFile = 0x0;
        uint64_t C_CommandLine__GetCommandParam        = 0x0;
        uint64_t C_CommandLine__GetCommandParamsCount  = 0x0;
        uint64_t C_CommandLine__ProcessCommandLine     = 0x0;

        // C_Ctx
        uint64_t C_Ctx__BeginUpdate = 0x0;
        uint64_t C_Ctx__EndUpdate   = 0x0;

        // C_DatabaseSystem
        uint64_t C_DatabaseSystem__GetDatabase = 0x0;

        // C_Door
        uint64_t C_Door__AILock        = 0x0;
        uint64_t C_Door__AIUnlock      = 0x0;
        uint64_t C_Door__Close         = 0x0;
        uint64_t C_Door__DisableAction = 0x0;
        uint64_t C_Door__EnableAction  = 0x0;
        uint64_t C_Door__Kick          = 0x0;
        uint64_t C_Door__Lock          = 0x0;
        uint64_t C_Door__Open          = 0x0;
        uint64_t C_Door__StartLockpick = 0x0;
        uint64_t C_Door__StopLockpick  = 0x0;
        uint64_t C_Door__ToggleOpen    = 0x0;
        uint64_t C_Door__Unlock        = 0x0;

        // C_Entity
        uint64_t C_Entity__Activate   = 0x0;
        uint64_t C_Entity__Deactivate = 0x0;
        uint64_t C_Entity__GameDone   = 0x0;
        uint64_t C_Entity__GameInit   = 0x0;
        uint64_t C_Entity__Release    = 0x0;

        // C_EntityFactory
        uint64_t C_EntityFactory__ComputeHash    = 0x0;
        uint64_t C_EntityFactory__CreateEntity   = 0x0;
        uint64_t C_EntityFactory__RegisterEntity = 0x0;

        // C_EntityList
        uint64_t C_EntityList__GetEntityList = 0x0;

        // C_Explosion
        uint64_t C_Explosion__Clear = 0x0;

        // C_Fader
        uint64_t C_Fader__FadeIn  = 0x0;
        uint64_t C_Fader__FadeOut = 0x0;
        uint64_t C_Fader__Reset   = 0x0;

        // C_Fire
        uint64_t C_Fire__Clear = 0x0;

        // C_Game
        uint64_t C_Game__GetGame = 0x0;

        // C_GameAudioModule
        uint64_t C_GameAudioModule__GetAudioModule    = 0x0;
        uint64_t C_GameAudioModule__SetCutsceneVolume = 0x0;
        uint64_t C_GameAudioModule__SetDialogueVolume = 0x0;
        uint64_t C_GameAudioModule__SetDynamicRange   = 0x0;
        uint64_t C_GameAudioModule__SetMasterVolume   = 0x0;
        uint64_t C_GameAudioModule__SetMusicVolume    = 0x0;
        uint64_t C_GameAudioModule__SetSfxVolume      = 0x0;

        // C_GameCamera
        uint64_t C_GameCamera__GetInstanceInternal = 0x0;
        uint64_t C_GameCamera__GetCamera           = 0x0;

        // C_GameDirector
        uint64_t C_GameDirector__GetDistrict = 0x0;

        // C_GameFramework
        uint64_t C_GameFramework__IsSuspended = 0x0;

        // C_GameGfxEnvEffModule
        uint64_t C_GameGfxEnvEffModule__GetCurrentWeatherSetName = 0x0;

        // C_GameGUI2Module
        uint64_t C_GameGUI2Module__GetDatabase                 = 0x0;
        uint64_t C_GameGUI2Module__GetGameGui2Module           = 0x0;
        uint64_t C_GameGUI2Module__Instance                    = 0x0;
        uint64_t C_GameGUI2Module__SendHUDSimpleBooleanMessage = 0x0;
        uint64_t C_GameGUI2Module__SendMessageMovie            = 0x0;

        // C_GameInputModule
        uint64_t C_GameInputModule__GetGameInputModule = 0x0;
        uint64_t C_GameInputModule__PauseInput         = 0x0;

        // C_GameTrafficModule
        uint64_t C_GameTrafficModule__GetInstance = 0x0;

        // C_GameRenderingSystemModule
        uint64_t C_GameRenderingSystemModule__GetGameRenderingSystemModule = 0x0;

        // C_GfxEnvironmentEffects
        uint64_t C_GfxEnvironmentEffects__Instance = 0x0;

        // C_HashName
        uint64_t C_HashName__ComputeHash = 0x0;
        uint64_t C_HashName__SetName     = 0x0;

        // C_Human2
        uint64_t C_Human2__EnableHumanClothes = 0x0;
        uint64_t C_Human2__EnableShadows      = 0x0;

        // C_Human2CarWrapper
        uint64_t C_Human2CarWrapper__GetSeatID = 0x0;
        uint64_t C_Human2CarWrapper__IsEngineOn = 0x0;

        // C_HumanInventory
        uint64_t C_HumanInventory__AddItem                       = 0x0;
        uint64_t C_HumanInventory__AddItemByData                 = 0x0;
        uint64_t C_HumanInventory__AddMedkits                    = 0x0;
        uint64_t C_HumanInventory__AddMoney                      = 0x0;
        uint64_t C_HumanInventory__AddWeapon                     = 0x0;
        uint64_t C_HumanInventory__CanAddAmmoByCategoryAux       = 0x0;
        uint64_t C_HumanInventory__CanAddAmmoByCategoryAuxAndMag = 0x0;
        uint64_t C_HumanInventory__CanAddUpgrade                 = 0x0;
        uint64_t C_HumanInventory__CanAddWeapon                  = 0x0;
        uint64_t C_HumanInventory__CanFire                       = 0x0;
        uint64_t C_HumanInventory__CanReload                     = 0x0;
        uint64_t C_HumanInventory__CanUseMedkit                  = 0x0;
        uint64_t C_HumanInventory__CreateDefaultItems            = 0x0;
        uint64_t C_HumanInventory__CreateModel                   = 0x0;
        uint64_t C_HumanInventory__DestroyModel                  = 0x0;
        uint64_t C_HumanInventory__DoReload                      = 0x0;
        uint64_t C_HumanInventory__DoShot                        = 0x0;
        uint64_t C_HumanInventory__DuplicateWeaponModel          = 0x0;
        uint64_t C_HumanInventory__GetLeftHandWeaponTableItem    = 0x0;
        uint64_t C_HumanInventory__GetRightHandWeaponTableItem   = 0x0;
        uint64_t C_HumanInventory__GetSelectedAmmoCategory       = 0x0;
        uint64_t C_HumanInventory__SelectAnimSetting             = 0x0;
        uint64_t C_HumanInventory__SelectByItemID                = 0x0;
        uint64_t C_HumanInventory__SelectNextWeapon              = 0x0;
        uint64_t C_HumanInventory__TellMedkit                    = 0x0;
        uint64_t C_HumanInventory__UseMedkit                     = 0x0;

        // C_HumanScript
        uint64_t C_HumanScript__GetOffVehicle  = 0x0;
        uint64_t C_HumanScript__GetOnVehicle   = 0x0;
        uint64_t C_HumanScript__ScrAim         = 0x0;
        uint64_t C_HumanScript__ScrAimAt       = 0x0;
        uint64_t C_HumanScript__ScrAttack      = 0x0;
        uint64_t C_HumanScript__SetHealth      = 0x0;
        uint64_t C_HumanScript__SetStealthMove = 0x0;

        // C_HumanSpawner
        uint64_t C_HumanSpawner__C_HumanSpawnerVtbl          = 0x0;
        uint64_t C_HumanSpawner__SetupDefaultArchetypeObject = 0x0;

        // C_HumanWeaponController
        uint64_t C_HumanWeaponController__DoShot                     = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponReloadDropMagazine = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponReloadInventory    = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponReloadShowMagazine = 0x0;
        uint64_t C_HumanWeaponController__DoWeaponSelectByItemId     = 0x0;
        uint64_t C_HumanWeaponController__GetAimDir                  = 0x0;
        uint64_t C_HumanWeaponController__GetRightHandWeaponID       = 0x0;
        uint64_t C_HumanWeaponController__GetShotPosDir              = 0x0;
        uint64_t C_HumanWeaponController__IsThrownWeapon             = 0x0;
        uint64_t C_HumanWeaponController__ResetScatterCoef           = 0x0;
        uint64_t C_HumanWeaponController__SetAiming                  = 0x0;
        uint64_t C_HumanWeaponController__SetCoverFlag               = 0x0;
        uint64_t C_HumanWeaponController__SetFirePressedFlag         = 0x0;
        uint64_t C_HumanWeaponController__SetStickMove               = 0x0;
        uint64_t C_HumanWeaponController__SetZoomFlag                = 0x0;

        // C_IE
        uint64_t C_IE__Alloc = 0x0;
        uint64_t C_IE__Free  = 0x0;

        // C_InjuryAnalyser
        uint64_t C_InjuryAnalyser__AddInjury = 0x0;

        // C_InventoryWrapper
        uint64_t C_InventoryWrapper__AddMoney  = 0x0;
        uint64_t C_InventoryWrapper__AddWeapon = 0x0;
        uint64_t C_InventoryWrapper__TellMoney = 0x0;

        // C_Keyboard
        uint64_t C_Keyboard__GetActiveModifiers = 0x0;
        uint64_t C_Keyboard__ToString           = 0x0;

        // C_MafiaCameraModule
        uint64_t C_MafiaCameraModule__GetMafiaCameraModule = 0x0;
        uint64_t C_MafiaCameraModule__GetPlayerCamera      = 0x0;

        // C_MafiaDBs
        uint64_t C_MafiaDBs__GetMafiaDBs         = 0x0;
        uint64_t C_MafiaDBs__GetTablesDatabase   = 0x0;
        uint64_t C_MafiaDBs__GetVehiclesDatabase = 0x0;

        // C_MafiaFramework
        uint64_t C_MafiaFramework__Instance = 0x0;

        // C_Matrix
        uint64_t C_Matrix__SetDir      = 0x0;
        uint64_t C_Matrix__SetDir2     = 0x0;
        uint64_t C_Matrix__SetDir3     = 0x0;
        uint64_t C_Matrix__SetRot      = 0x0;
        uint64_t C_Matrix__SetRotEuler = 0x0;

        // C_MenuSave
        uint64_t C_MenuSave__OpenDebugLoadChapterString = 0x0;

        // C_Motor
        uint64_t C_Motor__SetFuel = 0x0;

        // C_Navigation
        uint64_t C_Navigation__EnableGPSCustomPath       = 0x0;
        uint64_t C_Navigation__GetInstance               = 0x0;
        uint64_t C_Navigation__RegisterHumanPlayer       = 0x0;
        uint64_t C_Navigation__RegisterHumanPolice       = 0x0;
        uint64_t C_Navigation__RegisterVehicleCommon     = 0x0;
        uint64_t C_Navigation__RegisterVehicleEntity     = 0x0;
        uint64_t C_Navigation__RegisterVehicleMoto       = 0x0;
        uint64_t C_Navigation__RegisterVehiclePolice     = 0x0;
        uint64_t C_Navigation__RegisterVehiclePoliceBoat = 0x0;
        uint64_t C_Navigation__RegisterVehiclePoliceMoto = 0x0;
        uint64_t C_Navigation__RegisterVehicleTaxi       = 0x0;
        uint64_t C_Navigation__SetUserMark               = 0x0;
        uint64_t C_Navigation__UnregisterHuman           = 0x0;
        uint64_t C_Navigation__UnregisterId              = 0x0;
        uint64_t C_Navigation__UnregisterVehicle         = 0x0;

        // C_PlayerModelManager
        uint64_t C_PlayerModelManager__IsPlayerLoaded      = 0x0;
        uint64_t C_PlayerModelManager__SwitchPlayerProfile = 0x0;

        // C_PlayerTeleportModule
        uint64_t C_PlayerTeleportModule__GetPlayerTeleportModule = 0x0;
        uint64_t C_PlayerTeleportModule__Instance                = 0x0;
        uint64_t C_PlayerTeleportModule__TeleportPlayer          = 0x0;

        // C_ProfileSpawner
        uint64_t C_ProfileSpawner__C_ProfileSpawner      = 0x0;
        uint64_t C_ProfileSpawner__C_ProfileSpawnerDctor = 0x0;
        uint64_t C_ProfileSpawner__CreateActor           = 0x0;
        uint64_t C_ProfileSpawner__IsSpawnProfileLoaded  = 0x0;
        uint64_t C_ProfileSpawner__ReturnObject          = 0x0;

        // C_Quat
        uint64_t C_Quat__SetDir = 0x0;

        // C_RaceTimer
        uint64_t C_RaceTimer_SetVisible = 0x0;
        uint64_t C_RaceTimer_StartRace  = 0x0;

        // C_SceneObject
        uint64_t C_SceneObject__SetTransform = 0x0;

        // C_ShotManager
        uint64_t C_ShotManager__CreateExplosion = 0x0;
        uint64_t C_ShotManager__CreateFire      = 0x0;
        uint64_t C_ShotManager__GetInstance     = 0x0;

        // C_SlotWrapper
        uint64_t C_SlotWrapper__LoadData = 0x0;

        // C_StreamingModule
        uint64_t C_StreamingModule__SetStreamingPosSource = 0x0;

        // C_StreamingTrafficModule
        uint64_t C_StreamingTrafficModule__CloseSeason         = 0x0;
        uint64_t C_StreamingTrafficModule__GetSeasonOpened     = 0x0;
        uint64_t C_StreamingTrafficModule__GetInstance         = 0x0;
        uint64_t C_StreamingTrafficModule__OpenSeason          = 0x0;
        uint64_t C_StreamingTrafficModule__SetMaxHumanElements = 0x0;

        // C_StreamMap
        uint64_t C_StreamMap__CloseGame    = 0x0;
        uint64_t C_StreamMap__CloseMission = 0x0;
        uint64_t C_StreamMap__ClosePart    = 0x0;
        uint64_t C_StreamMap__GetGame      = 0x0;
        uint64_t C_StreamMap__GetMission   = 0x0;
        uint64_t C_StreamMap__GetPart      = 0x0;
        uint64_t C_StreamMap__OpenGame     = 0x0;
        uint64_t C_StreamMap__OpenMission  = 0x0;
        uint64_t C_StreamMap__OpenPart     = 0x0;

        // C_String
        uint64_t C_String__SetString = 0x0;

        // C_SysODB
        uint64_t C_SysODB__GetInstance = 0x0;

        // C_TickedModuleManager
        uint64_t C_TickedModuleManager__GetTickedModuleManager = 0x0;

        // C_TrafficSpawnManager
        uint64_t C_TrafficSpawnManager__GetCivilCarDensityMult = 0x0;
        uint64_t C_TrafficSpawnManager__Populate             = 0x0;
        uint64_t C_TrafficSpawnManager__SetTrainDensity      = 0x0;
        uint64_t C_TrafficSpawnManager__SwitchAmbientTraffic = 0x0;
        uint64_t C_TrafficSpawnManager__UpdateMaxElementsCnt = 0x0;

        // C_Translocator
        uint64_t C_Translocator__SpawnObject = 0x0;
        uint64_t C_Translocator__SpawnObjects = 0x0;

        // C_TranslocableObjectSpawner
        uint64_t C_TranslocableObjectSpawner__BeginUpdate = 0x0;
        uint64_t C_TranslocableObjectSpawner__EndUpdate   = 0x0;
        uint64_t C_TranslocableObjectSpawner__Spawn = 0x0;
        uint64_t C_TranslocableObjectSpawner__Update      = 0x0;

        // C_Vehicle
        uint64_t C_Vehicle__AddVehicleFlags    = 0x0;
        uint64_t C_Vehicle__ChangeRadioStation = 0x0;
        uint64_t C_Vehicle__ClearVehicleFlags  = 0x0;
        uint64_t C_Vehicle__Damage             = 0x0;
        uint64_t C_Vehicle__DamageBreaks       = 0x0;
        uint64_t C_Vehicle__EnableRadio        = 0x0;
        uint64_t C_Vehicle__GetSPZText         = 0x0;
        uint64_t C_Vehicle__IsActive           = 0x0;
        uint64_t C_Vehicle__IsAnyLightOn       = 0x0;
        uint64_t C_Vehicle__IsSiren            = 0x0;
        uint64_t C_Vehicle__SetActive          = 0x0;
        uint64_t C_Vehicle__SetAngularSpeed    = 0x0;
        uint64_t C_Vehicle__SetBeaconLightsOn  = 0x0;
        uint64_t C_Vehicle__SetBrake           = 0x0;
        uint64_t C_Vehicle__SetDoorFree        = 0x0;
        uint64_t C_Vehicle__SetEngineOn        = 0x0;
        uint64_t C_Vehicle__SetGear            = 0x0;
        uint64_t C_Vehicle__SetHandbrake       = 0x0;
        uint64_t C_Vehicle__SetHorn            = 0x0;
        uint64_t C_Vehicle__SetIndicatorLightsOn = 0x0;
        uint64_t C_Vehicle__SetInteriorColors  = 0x0;
        uint64_t C_Vehicle__SetPower           = 0x0;
        uint64_t C_Vehicle__SetReflectorLightsOn = 0x0;
        uint64_t C_Vehicle__SetSearchLightsOn  = 0x0;
        uint64_t C_Vehicle__SetSiren           = 0x0;
        uint64_t C_Vehicle__SetSpeed           = 0x0;
        uint64_t C_Vehicle__SetSpeedLimit      = 0x0;
        uint64_t C_Vehicle__SetSPZText         = 0x0;
        uint64_t C_Vehicle__SetSteer           = 0x0;
        uint64_t C_Vehicle__SetTaxiLightsOn    = 0x0;
        uint64_t C_Vehicle__SetVehicleColor    = 0x0;
        uint64_t C_Vehicle__SetVehicleDirty    = 0x0;
        uint64_t C_Vehicle__SetVehicleMatrix   = 0x0;
        uint64_t C_Vehicle__SetVehicleRust     = 0x0;
        uint64_t C_Vehicle__SetWheelColor      = 0x0;
        uint64_t C_Vehicle__SetWindowTintColor = 0x0;
        uint64_t C_Vehicle__TurnRadioOn        = 0x0;

        // C_VehiclesDatabase
        uint64_t C_VehiclesDatabase__GetVehicleByID    = 0x0;
        uint64_t C_VehiclesDatabase__GetVehicleByIndex = 0x0;
        uint64_t C_VehiclesDatabase__GetVehicleByModel = 0x0;
        uint64_t C_VehiclesDatabase__GetVehiclesCount  = 0x0;

        // C_VehicleRealLightManager
        uint64_t C_VehicleRealLightManager__SetLightIntensity      = 0x0;
        uint64_t C_VehicleRealLightManager__SetPlayerLights        = 0x0;
        uint64_t C_VehicleRealLightManager__SetReflectorLightsOn   = 0x0;
        uint64_t C_VehicleRealLightManager__SetVisible             = 0x0;

        // C_WAnimPlaybackManager
        uint64_t C_WAnimPlaybackManager__PlayState = 0x0;

        // C_WeatherManager2
        uint64_t C_WeatherManager2__EnableTimeFlow              = 0x0;
        uint64_t C_WeatherManager2__GetDayTimeHours             = 0x0;
        uint64_t C_WeatherManager2__GetDayTimeRel               = 0x0;
        uint64_t C_WeatherManager2__GetDefaultTimeFlowSpeedMult = 0x0;
        uint64_t C_WeatherManager2__GetUserTimeFlowSpeedMult    = 0x0;
        uint64_t C_WeatherManager2__IsTimeFlowEnabled           = 0x0;
        uint64_t C_WeatherManager2__SetDayTimeHours             = 0x0;
        uint64_t C_WeatherManager2__SetDayTimeSec               = 0x0;
        uint64_t C_WeatherManager2__SetDefaultTimeFlowSpeedMult = 0x0;
        uint64_t C_WeatherManager2__SetUserTimeFlowSpeedMult    = 0x0;
        uint64_t C_WeatherManager2__SetWeatherSet               = 0x0;

        // I_Core
        uint64_t I_Core__GetInstance = 0x0;

        // I_GameDirector
        uint64_t I_GameDirector__GetInstance = 0x0;

        // I_VirtualFileSystemCache
        uint64_t I_VirtualFileSystemCache__GetInstance = 0x0;

        // I_StreamMap
        uint64_t I_StreamMap__GetInstance = 0x0;

        // Lua
        uint64_t Lua__isstring   = 0x0;
        uint64_t Lua__loadbuffer = 0x0;
        uint64_t Lua__pcall      = 0x0;
        uint64_t Lua__tostring   = 0x0;

        // init
        uint64_t C_InitDone_MafiaFramework = 0x0;
        uint64_t LoadIntro                 = 0x0;

        // TODO: rename me
        uint64_t renameme__SpawnObject  = 0x0;
        uint64_t renameme__SpawnObject2 = 0x0;
        uint64_t renameme__SpawnObject3 = 0x0;

        // Script
        uint64_t Script__GetWeaponIdByName = 0x0;

        static void InitPatterns();
    };

    extern Patterns gPatterns;
} // namespace SDK
