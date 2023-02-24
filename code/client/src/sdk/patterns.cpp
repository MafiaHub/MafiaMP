#include "patterns.h"

namespace SDK {
    Patterns gPatterns;

    void Patterns::InitPatterns() {
        gPatterns.C_Entity__GameInitAddr   = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8B D9 8B 49 ? 8B C1 C1 E8 ? A8 ?").get_first());
        gPatterns.C_Entity__GameDoneAddr   = reinterpret_cast<uint64_t>(hook::pattern("40 57 48 83 EC ? 8B 41 ? 48 8B F9 25 ? ? ? ?").get_first());
        gPatterns.C_Entity__ActivateAddr   = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8B D9 8B 49 ? 8B C1 25 ? ? ? ?").get_first());
        gPatterns.C_Entity__DeactivateAddr = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 8B 41 ? 48 8B D9 25 ? ? ? ? 3D ? ? ? ?").get_first());
        gPatterns.C_Entity__ReleaseAddr    = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC ? 8B 41 ? 48 8B D9 25 ? ? ? ?").get_first());

        gPatterns.C_Game__GetGame = hook::get_opcode_address("E8 ? ? ? ? 0F B7 5B 20");

        gPatterns.C_GameInputModule__GetGameInputModule = hook::get_opcode_address("E8 ? ? ? ? 4C 8B F0 8B D7");
        gPatterns.C_GameInputModule__PauseInput         = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 08 C6 80 ? ? ? ? ? 48 8B 5C 24 ?");

        gPatterns.C_EntityList__GetEntityList = hook::get_opcode_address("E8 ? ? ? ? 8B 53 40 48 8B C8");

        gPatterns.C_PlayerModelManager__IsPlayerLoaded          = reinterpret_cast<uint64_t>(hook::pattern("48 8B 49 28 48 85 C9 75 03 ").get_first());
        gPatterns.C_PlayerModelManager__SwitchPlayerProfileAddr = hook::get_opcode_address("E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 41 48");

        gPatterns.C_ActorsSlotWrapper__C_ActorsSlotWrapperAddr  = reinterpret_cast<uint64_t>(hook::pattern("48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 74 24 ? 48 8D 05 ? ? ? ? 33 ED").get_first());
        gPatterns.C_ActorsSlotWrapper__UpdateToCreateActorsAddr = hook::get_opcode_address("E8 ? ? ? ? EB 25 83 FF 0A");
        gPatterns.C_ActorsSlotWrapper__GetFreeActorAddr         = reinterpret_cast<uint64_t>(hook::pattern("41 56 48 83 EC ? 33 C0 48 8B F9").get_first(-0xA));
        gPatterns.C_ActorsSlotWrapper__ReturnActorAddr          = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 56 48 83 EC ? 48 8B 41 ? 40 32 ED").get_first());
        gPatterns.C_ActorsSlotWrapper__CloseAddr                = reinterpret_cast<uint64_t>(hook::pattern("40 53 55 48 83 EC ? C7 41 ? ? ? ? ?").get_first());

        gPatterns.C_SlotWrapper__LoadDataAddr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 41 ? 41 8B F9 49 8B F0 48 8B EA").get_first());

        gPatterns.C_StreamingModule__SetStreamingPosSourceAddr = reinterpret_cast<uint64_t>(hook::pattern("33 C0 89 91 ? ? ? ? 48 89 81 ? ? ? ?").get_first());

        gPatterns.C_HumanSpawner__C_HumanSpawnerVtblAddr          = hook::get_opcode_address("49 8B CE E8 ? ? ? ? 48 8B 5C 24 ? 48 8D 05 ? ? ? ?", 15);
        gPatterns.C_HumanSpawner__SetupDefaultArchetypeObjectAddr = reinterpret_cast<uint64_t>(hook::pattern("4C 8B DC 49 89 5B ? 56 57 41 56 48 83 EC ? 48 8B 1D ? ? ? ?").get_first());

        gPatterns.C_ProfileSpawner__C_ProfileSpawnerAddr      = hook::get_opcode_address("49 8B CE E8 ? ? ? ? 48 8B 5C 24 ? 48 8D 05 ? ? ? ?", 3);
        gPatterns.C_ProfileSpawner__C_ProfileSpawnerDctorAddr = reinterpret_cast<uint64_t>(hook::pattern("40 53 56 57 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B D9").get_first());
        gPatterns.C_ProfileSpawner__IsSpawnProfileLoadedAddr  = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B C8 48 8D 54 24 ? E8 ? ? ? ? 4C 8D 83 ? ? ? ?").get_first());
        gPatterns.C_ProfileSpawner__CreateActorAddr           = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? BA ? ? ? ? 48 89 4B 08");
        gPatterns.C_ProfileSpawner__ReturnObjectAddr          = hook::get_opcode_address("E8 ? ? ? ? EB 6F 48 8B 5F 08");

        gPatterns.C_GameCamera__GetInstanceInternalAddr = hook::get_opcode_address("E8 ? ? ? ? 48 8B C8 4C 8D 4D 30");

        gPatterns.C_WeatherManager2__GetDayTimeHoursAddr = reinterpret_cast<uint64_t>(hook::pattern("F3 0F 10 81 ? ? ? ? F3 0F 59 05 ? ? ? ? C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? F3 0F 10 81 ? ? ? ?").get_first());
        gPatterns.C_WeatherManager2__GetDayTimeRelAddr   = reinterpret_cast<uint64_t>(hook::pattern("F3 0F 10 81 ? ? ? ? F3 0F 59 05 ? ? ? ? C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 48 8B 41 ?").get_first());
        gPatterns.C_WeatherManager2__SetDayTimeHoursAddr = reinterpret_cast<uint64_t>(hook::pattern("F3 0F 59 0D ? ? ? ? 0F 57 DB F3 0F 10 15 ? ? ? ? 0F 28 C1 F3 0F 5C C2 0F 2F C3 73 ? 0F 28 D1 F3 0F 10 0D ? ? ? ? 0F 28 C1 F3 "
                                                                                                    "0F 5C C2 0F 2F C3 72 ? F3 0F 11 89 ? ? ? ? C3 F3 0F 11 91 ? ? ? ? C3 ? ? ? ? ? ? ? ? F3 0F 59 0D ? ? ? ?")
                                                                                          .get_first());
        gPatterns.C_WeatherManager2__SetDayTimeRelAddr   = reinterpret_cast<uint64_t>(hook::pattern("F3 0F 59 0D ? ? ? ? F3 0F 11 49 ? C3 ? ? F3 0F 59 0D ? ? ? ? 0F 57 DB").get_first());
        gPatterns.C_WeatherManager2__SetWeatherSetAddr   = hook::get_opcode_address("E8 ? ? ? ? 48 8D 4F ? E8 ? ? ? ? E9 ? ? ? ? E8 ? ? ? ?");

        gPatterns.I_Core__GetInstance             = hook::get_opcode_address("E8 ? ? ? ? 4C 8B 40 68");
        gPatterns.C_SceneObject__SetTransformAddr = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8D 41 ? 48 8B D9 0F 10 02").get_first());

        gPatterns.renameme__SpawnObject  = hook::get_opcode_address("E8 ? ? ? ? 33 F6 EB 9F");
        gPatterns.renameme__SpawnObject2 = hook::get_opcode_address("E8 ? ? ? ? 49 8B 4C 3F ?");
        gPatterns.renameme__SpawnObject3 = hook::get_opcode_address("E8 ? ? ? ? 4C 8B E8 49 8B CD");

        gPatterns.I_VirtualFileSystemCache__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 41 0F B7 CF");

        gPatterns.I_GameDirector__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 48 8B 57 18 84 DB");
        gPatterns.C_GameDirector__GetDistrict = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 50 F2 0F 10 02").get_first());

        //NOTE: extract address from another function and its instruction
        uint64_t C_MafiaFramework_Addr       = hook::get_opcode_address("E8 ? ? ? ? E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 33 D2 48 8B C8") + 0xD;
        uint8_t *C_MafiaFramework_Addr_Bytes = reinterpret_cast<uint8_t *>(C_MafiaFramework_Addr);
        gPatterns.C_MafiaFramework__Instance = reinterpret_cast<uint64_t>(C_MafiaFramework_Addr_Bytes + *(int32_t *)(C_MafiaFramework_Addr_Bytes + 3) + 7);

        uint64_t C_PlayerTeleportModule_Addr       = hook::get_opcode_address("E8 ? ? ? ? 83 78 38 00 74 13") + 0xD;
        uint8_t *C_PlayerTeleportModule_Addr_Bytes = reinterpret_cast<uint8_t *>(C_PlayerTeleportModule_Addr);
        gPatterns.C_PlayerTeleportModule__Instance = reinterpret_cast<uint64_t>(C_PlayerTeleportModule_Addr_Bytes + *(int32_t *)(C_PlayerTeleportModule_Addr_Bytes + 3) + 7);

        gPatterns.C_PlayerTeleportModule__TeleportPlayer = reinterpret_cast<uint64_t>(hook::pattern("40 56 48 83 EC 50 83 79 38 00").get_first());

        uint64_t C_GfxEnvironmentEffects_Addr      = reinterpret_cast<uint64_t>(hook::pattern("48 8B 05 ? ? ? ? 48 8B 40 28 48 8B 40 28").get_first());
        uint8_t *C_GfxEnvironmentEffects_Bytes     = reinterpret_cast<uint8_t *>(C_GfxEnvironmentEffects_Addr);
        gPatterns.C_GfxEnvironmentEffects_Instance = reinterpret_cast<uint64_t>(C_GfxEnvironmentEffects_Bytes + *(int32_t *)(C_GfxEnvironmentEffects_Bytes + 3) + 7);

        uint64_t C_GameGui2Module_Addr      = hook::get_opcode_address("E8 ? ? ? ? 41 8D 56 11");
        uint8_t *C_GameGui2Module_Bytes     = reinterpret_cast<uint8_t *>(C_GameGui2Module_Addr);
        gPatterns.C_GameGUI2Module_Instance = reinterpret_cast<uint64_t>(C_GameGui2Module_Bytes + *(int32_t *)(C_GameGui2Module_Bytes + 3) + 7);

        gPatterns.C_Matrix__SetDir2Addr     = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? F2 0F 10 02 48 8B D9 8B 42 ?").get_first());
        gPatterns.C_Matrix__SetDirAddr      = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 89 43 ? 89 43 ? 89 43 ? 48 83 C4 ? 5B C3 ? 40 53 48 83 EC ? 48 8B D9 48 8D 4C 24 ?").get_first());
        gPatterns.C_Matrix__SetDir3Addr     = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? F3 0F 10 02 0F 57 DB").get_first());
        gPatterns.C_Matrix__SetRotAddr      = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 89 43 ? 89 43 ? 89 43 ? 48 83 C4 ? 5B C3 ? 40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ?").get_first());
        gPatterns.C_Matrix__SetRotEulerAddr = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 89 43 ? 89 43 ? 89 43 ? 48 83 C4 ? 5B C3 ? 0F 10 02").get_first());

        gPatterns.C_HashName__ComputeHashAddr = reinterpret_cast<uint64_t>(hook::pattern("48 89 74 24 ? 41 56 48 83 EC ? 4C 8B F2 48 8B F1 48 85 C9").get_first());
        gPatterns.C_HashName__SetNameAddr     = reinterpret_cast<uint64_t>(hook::pattern("44 0F B6 11 4C 8B C2").get_first());

        gPatterns.C_String__SetStringAddr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B F1 48 85 D2 75 ? 48 8B 09").get_first());

        gPatterns.C_IE__AllocAddr = hook::get_opcode_address("48 ? ? 48 ? ? ? ? B9 04 14 00 00 E8 ? ? ? ? 48", 0xD);
        gPatterns.C_IE__FreeAddr  = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 41 B8 0D 00 00 00 48 ? ? ? ? ? ? 48");

        gPatterns.C_TickedModuleManager__GetTickedModuleManager = hook::get_opcode_address("E8 ? ? ? ? 45 8B 46 24");

        gPatterns.C_Ctx__BeginUpdateAddr = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 48 89 51 ? 48 8D 4C 24 ?").get_first());
        gPatterns.C_Ctx__EndUpdateAddr   = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 83 79 ? ? 48 8B D9 74 ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D 4C 24 ?").get_first());

        gPatterns.C_CharacterController__ActivateHandler                           = hook::get_opcode_address("E8 ? ? ? ? 48 8D 56 74");
        gPatterns.C_CharacterController__DeactivateHandler_FromPlayerInput         = hook::get_opcode_address("E8 ? ? ? ? EB 0F 48 8B 93 ? ? ? ?");
        gPatterns.C_CharacterStateHandlerBaseLocomotion__Idle2MoveTransitionActive = hook::get_opcode_address("E8 ? ? ? ? 48 89 75 E0 C7 45 ? ? ? ? ?");
        gPatterns.C_CharacterStateHandlerBaseLocomotion__AddRemoveSprintDescriptor = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 08 45 33 FF");
        gPatterns.C_CharacterStateHandlerMove__SharpTurnTransitionActive           = hook::get_opcode_address("E8 ? ? ? ? 84 C0 74 60 40 84 FF");
        gPatterns.C_WAnimPlaybackManager__PlayState                                = hook::get_opcode_address("E8 ? ? ? ? 4C 39 7F 50");
        gPatterns.C_BehaviorCharacter__SetWAnimVariable_float                      = hook::get_opcode_address("E9 ? ? ? ? 83 FA 20");
        gPatterns.C_CharacterController__TriggerActorActionById                    = hook::get_opcode_address("E8 ? ? ? ? 45 0F 2F 87 ? ? ? ?");

        // C_HumanScript
        gPatterns.C_HumanScript__GetOnVehicle  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 48 8B 5C 24 ? 48 85 C9 74 ? 83 69 ? ? 75 ? 48 8B 01 FF 50 ? 48 8B 06");
        gPatterns.C_HumanScript__GetOffVehicle = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? 83 69 ? ? 75 ? 48 8B 01 FF 50 ? 48 8B 8B ? ? ? ? 4C 8D 05 ? ? ? ?");
        gPatterns.C_HumanScript__SetHealth     = reinterpret_cast<uint64_t>(hook::pattern("48 83 EC ? 48 8B 09 0F 29 74 24 ?").get_first());

        // C_Quat
        gPatterns.C_Quat__SetDir = hook::get_opcode_address("E8 ? ? ? ? F3 44 0F 59 5D ?");

        // C_Car
        gPatterns.C_Car__Lock   = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? B8 ? ? ? ? C3");
        gPatterns.C_Car__Unlock = hook::get_opcode_address("E8 ? ? ? ? C6 43 ? ? 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? ? C2 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? C2 ? ?");

        gPatterns.C_Car__LockEntryPoints   = hook::get_opcode_address("E8 ? ? ? ? 45 8D 7E ? 48 8B 4F ?");
        gPatterns.C_Car__UnlockEntryPoints = hook::get_opcode_address("E8 ? ? ? ? 48 8B 6C 24 ? 48 83 C4 ? 5E C3 ? ? ? ? ? ? ? ? ? 48 89 5C 24 ?");

        //NOTE: new version only
#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Car__OpenHood = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 40 53 48 83 EC ? 45 33 C0 33 D2 48 8B D9 E8 ? ? ? ? 48 8B 43 ? C7 40 ? ? ? ? "
                                                             "? C7 00 ? ? ? ? 48 83 C0 ? 48 89 43 ? B8 ? ? ? ? 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? 48 83 EC ?");
#endif

        gPatterns.C_Car__CloseHood = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 81 ? ? ? ? 33 DB 48 2B 81 ? ? ? ? 33 FF 48 C1 F8 ? 48 8B F1 4C 63 F0 85 C0 0F 8E ? ? ? "
                                                                              "? 48 89 6C 24 ? 0F 29 74 24 ? 0F 57 F6 66 66 0F 1F 84 00 ? ? ? ? 48 8B 86 ? ? ? ? 48 63 0C B8 48 8B 86 ? ? ? ? 48 8B 14 C8 33 C9 8B 42 ? 39 05 ? ? ? ? "
                                                                              "48 0F 45 CA 48 85 C9 74 ? 48 0F BF 49 ? 48 8B 86 ? ? ? ? 48 8B 14 C8 0F 2F 72 ? 73 ? 0F B6 86 ? ? ? ? 48 8D 8E ? ? ? ? C0 E8 ? 45 33 C0")
                                                                    .get_first());

        gPatterns.C_Car__OpenTrunk  = hook::get_opcode_address("E8 ? ? ? ? 4D 85 F6 74 ? 49 8D 9E ? ? ? ?");
        gPatterns.C_Car__CloseTrunk = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4F ? 4C 8D 47 ? BA ? ? ? ? E8 ? ? ? ? 84 C0 0F 84 ? ? ? ?");

        gPatterns.C_Car__SetMotorDamage = hook::get_opcode_address("E8 ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 49 21 86 ? ? ? ?");
        gPatterns.C_Car__GetMotorDamage = hook::get_opcode_address("E8 ? ? ? ? 49 8B CE 0F 28 F8 E8 ? ? ? ? 0F 28 F0");

        gPatterns.C_Car__GetDamage       = hook::get_opcode_address("E8 ? ? ? ? 0F 57 C9 0F 2E C1 75 ? 0F 28 C1");
        gPatterns.C_Car__SetTransparency = hook::get_opcode_address("E8 ? ? ? ? 4C 39 2D ? ? ? ?");
        gPatterns.C_Car__SetSpeed        = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 33 C0 48 83 C4 ? 5F C3 ? ? 48 83 EC ? 48 8B C2");
        gPatterns.C_Car__SetSeatStatus   = hook::get_opcode_address("E8 ? ? ? ? 45 85 F6 8B C5");
        gPatterns.C_Car__ExplodeCar      = hook::get_opcode_address("E8 ? ? ? ? 48 8B 86 ? ? ? ? 45 33 E4 48 2B 86 ? ? ? ?");
        gPatterns.C_Car__ExplodeCar_2    = reinterpret_cast<uint64_t>(hook::pattern("F3 0F 11 4C 24 ? 4C 8B DC 55").get_first());

#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Car__PosefujZimuVShopu = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? 4C 8B 41 ?");
#else
        gPatterns.C_Car__PosefujZimuVShopu = hook::get_opcode_address("E8 ? ? ? ? 8B 55 60 83 FA FF");
#endif

#ifndef NONSTEAM_SUPPORT
        // TODO lol
        gPatterns.C_Car__RestoreCar = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4B 10 0F 57 C9");
#else
        //NOTE: todo
        gPatterns.C_Car__RestoreCar   = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4B 10 0F 57 C9");
#endif

        // C_Motor
#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Motor__SetFuel = hook::get_opcode_address("E8 ? ? ? ? 33 C0 0F 28 74 24 ? 48 83 C4 ? 5B C3 ? ? ? 48 89 5C 24 ?");
#else
        gPatterns.C_Motor__SetFuel = hook::get_opcode_address("E8 ? ? ? ? 41 0F 10 96 ? ? ? ?");
#endif

        // C_Vehicle
        gPatterns.C_Vehicle__AddVehicleFlags   = hook::get_opcode_address("E8 ? ? ? ? 40 F6 C7 08 74 0D");
        gPatterns.C_Vehicle__ClearVehicleFlags = hook::get_opcode_address("E8 ? ? ? ? 40 80 FE 04");
        gPatterns.C_Vehicle__SetVehicleMatrix  = hook::get_opcode_address("E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0B");
        gPatterns.C_Vehicle__SetSPZText        = hook::get_opcode_address("E8 ? ? ? ? 48 8D 5D ? BF ? ? ? ? 0F B7 13");
        gPatterns.C_Vehicle__GetSPZText        = hook::get_opcode_address("E8 ? ? ? ? 49 8D 4F ? 48 8B D0");
        gPatterns.C_Vehicle__SetActive         = hook::get_opcode_address("E8 ? ? ? ? F3 0F 59 35 ? ? ? ? 48 8D 8B ? ? ? ?");

        //NOTE: new version only
#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Vehicle__Damage = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 8B 5C 24 ? 48 83 C4 ? 5F C3 ? ? ? ? ? ? B8 ? ? ? ?");
#else
        // TODO
#endif

        gPatterns.C_Vehicle__SetBeaconLightsOn  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 7C 24 ? 48 83 C4 ? 5B C3 48 8B D9");
        gPatterns.C_Vehicle__EnableRadio        = hook::get_opcode_address("E8 ? ? ? ? 49 8B 84 24 ? ? ? ? 49 8B F7");
        gPatterns.C_Vehicle__SetBrake           = hook::get_opcode_address("E8 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? 83 A7 ? ? ? ? ?");
        gPatterns.C_Vehicle__SetEngineOn        = hook::get_opcode_address("E8 ? ? ? ? 48 8B 9E ? ? ? ? 0F B6 C3");
        gPatterns.C_Vehicle__SetGear            = hook::get_opcode_address("E8 ? ? ? ? C6 83 ? ? ? ? ? 80 7F ? ? 74 ? 33 D2");
        gPatterns.C_Vehicle__SetHandbrake       = hook::get_opcode_address("E8 ? ? ? ? 49 8B 86 ? ? ? ? 4D 89 BE ? ? ? ?");
        gPatterns.C_Vehicle__SetPower           = hook::get_opcode_address("E8 ? ? ? ? F3 0F 10 8B ? ? ? ? 45 33 C0 48 8B CF E8 ? ? ? ?");
        gPatterns.C_Vehicle__SetSearchLightsOn  = hook::get_opcode_address("E8 ? ? ? ? 80 7E ? ? 0F 84 ? ? ? ? E8 ? ? ? ?");
        gPatterns.C_Vehicle__SetSiren           = hook::get_opcode_address("E8 ? ? ? ? 33 D2 48 8B CE E8 ? ? ? ? 48 8B 0D ? ? ? ?");
        gPatterns.C_Vehicle__SetSpeedLimit      = hook::get_opcode_address("E8 ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9 74 ? 48 8B 89 ? ? ? ?");
        gPatterns.C_Vehicle__SetSteer           = hook::get_opcode_address("E8 ? ? ? ? 45 33 C9 44 89 A7 ? ? ? ?");
        gPatterns.C_Vehicle__SetHorn            = hook::get_opcode_address("E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B F0");
        gPatterns.C_Vehicle__TurnRadioOn        = reinterpret_cast<uint64_t>(hook::pattern("48 83 B9 ? ? ? ? ? 44 0F B6 CA").get_first());
        gPatterns.C_Vehicle__DamageBreaks       = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? 48 8B 01 48 8B D9 0F 29 7C 24 ? 0F 28 F9 FF 50 ? 84 C0 74 ? 48 8B 5B ? 4C 8D 44 24 ? 0F 29 74 24 ? 48 8D 54 "
                                                                                                 "24 ? 0F 57 F6 C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? 48 8D 8B ? ? ? ? E8 ? ? ? ? F3 0F 10 0D ? ? ? ? 0F 28 C7 F3 0F 5C C1 0F "
                                                                                                 "2F C6 73 ? 0F 28 CF 0F 28 C1 0F 57 05 ? ? ? ? 0F 2F C6 73 ? 0F 28 F1 F3 0F 59 35 ? ? ? ? 48 8D 8B ? ? ? ? F3 0F 10 54 24 ?")
                                                                           .get_first());
        gPatterns.C_Vehicle__SetDoorFree        = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 33 C0 48 8B 74 24 ? 48 83 C4 ? 5F C3 ? ? ? 40 53 48 83 EC ? 45 33 C0 33 D2 48 8B D9 E8 ? ? ? ? 48 8B 43 ? C7 40 ? ? "
                                                                                  "? ? ? C7 00 ? ? ? ? 48 83 C0 ? 48 89 43 ? B8 ? ? ? ? 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? B8 ? ? ? ?");
        gPatterns.C_Vehicle__SetSpeed           = hook::get_opcode_address("E8 ? ? ? ? 49 8B CF E8 ? ? ? ? 45 0F 57 C9");
        gPatterns.C_Vehicle__SetAngularSpeed    = hook::get_opcode_address(" E8 ? ? ? ? E9 ? ? ? ? 80 BD ? ? ? ? ? 0F 85 ? ? ? ?");
        gPatterns.C_Vehicle__IsActive           = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 0A B2 01");
        gPatterns.C_Vehicle__IsSiren            = hook::get_opcode_address("E8 ? ? ? ? 0F B6 4D BF");
        gPatterns.C_Vehicle__SetVehicleDirty    = hook::get_opcode_address("E9 ? ? ? ? ? ? ? ? ? ? ? ? ? ? 40 57 48 83 EC ? 48 8B 81 ? ? ? ?");
        gPatterns.C_Vehicle__SetVehicleRust     = hook::get_opcode_address("E9 ? ? ? ? ? ? ? ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56");
        gPatterns.C_Vehicle__SetVehicleColor    = hook::get_opcode_address("E8 ? ? ? ? 8B 43 ? 89 87 ? ? ? ? 8B 43 ? 89 87 ? ? ? ? 83 7B ? ?");
        gPatterns.C_Vehicle__SetInteriorColors  = hook::get_opcode_address("E8 ? ? ? ? 48 81 C4 ? ? ? ? 41 5E 41 5D 5D C3 ? ? ? ? ? ? ? ? ? ? 40 55");
        gPatterns.C_Vehicle__SetWindowTintColor = hook::get_opcode_address("E8 ? ? ? ? 89 AE ? ? ? ? 4C 8D 9C 24 ? ? ? ?");
        gPatterns.C_Vehicle__SetWheelTintColor  = hook::get_opcode_address("E8 ? ? ? ? 45 89 B7 ? ? ? ? 41 89 B7 ? ? ? ?");

        // C_Door
        gPatterns.C_Door__Lock          = reinterpret_cast<uint64_t>(hook::pattern("F6 81 ? ? ? ? ? 48 8B D1 75 ? 83 B9 ? ? ? ? ? 74 ? 8B 89 ? ? ? ? 85 C9").get_first());
        gPatterns.C_Door__AILock        = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? F6 81 ? ? ? ? ? 48 8B D9 75 ? 83 B9 ? ? ? ? ? 74 ? E8 ? ? ? ? 48 8B C8 41 B0 ? 48 8B D3 E8 ? ? ? ? 83 8B ? ? ? ? ?").get_first());
        gPatterns.C_Door__Unlock        = hook::get_opcode_address("E8 ? ? ? ? EB ? 83 F8 ? 75 ? 48 8B 83 ? ? ? ?");
        gPatterns.C_Door__AIUnlock      = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC ? F6 81 ? ? ? ? ? 48 8B D9 75 ? 83 B9 ? ? ? ? ? 74 ? E8 ? ? ? ? 48 8B C8 41 B0 ? 48 8B D3 E8 ? ? ? ? 83 A3 ? ? ? ? ?").get_first());
        gPatterns.C_Door__StartLockpick = reinterpret_cast<uint64_t>(hook::pattern("F6 81 ? ? ? ? ? 75 ? 83 B9 ? ? ? ? ? 74 ? 48 81 C1 ? ? ? ? E9 ? ? ? ?").get_first());
        gPatterns.C_Door__StopLockpick  = reinterpret_cast<uint64_t>(hook::pattern("F6 81 ? ? ? ? ? 75 ? 83 B9 ? ? ? ? ? 74 ? 83 B9 ? ? ? ? ? 75 ? 45 33 C0").get_first());
        gPatterns.C_Door__Kick          = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 48 8B 42 ? 8B 48 ?");
        gPatterns.C_Door__Open          = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 83 B9 ? ? ? ? ? 0F 85 ? ? ? ?");
        gPatterns.C_Door__Close         = hook::get_opcode_address("E8 ? ? ? ? C6 86 ? ? ? ? ? 33 D2");
        gPatterns.C_Door__ToggleOpen    = hook::get_opcode_address("E8 ? ? ? ? EB ? 80 7A ? ? 74 ?");
        gPatterns.C_Door__EnableAction  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 33 C0 48 83 C4 ? C3 ? ? ? ? ? ? ? ? ? ? ? ? ? 48 83 EC ?");
        gPatterns.C_Door__DisableAction = reinterpret_cast<uint64_t>(hook::pattern("83 A1 ? ? ? ? ? 48 8B 81 ? ? ? ? 48 85 C0").get_first());

        // C_Human2CarWrapper
        gPatterns.C_Human2CarWrapper__GetSeatID = hook::get_opcode_address("E8 ? ? ? ? 3D ? ? ? ? 75 0E");

        // C_InventoryWrapper
        gPatterns.C_InventoryWrapper__AddMoney  = reinterpret_cast<uint64_t>(hook::pattern("40 53 48 83 EC 20 48 8B 41 68 48 8B DA 80 78 18 09 75 4E").get_first());
        gPatterns.C_InventoryWrapper__AddWeapon = hook::get_opcode_address("E8 ? ? ? ? 41 38 76 4C 74 0C");
        gPatterns.C_InventoryWrapper__TellMoney = reinterpret_cast<uint64_t>(hook::pattern("48 83 EC 28 48 8B 41 68 80 78 18 09").get_first());

        // C_HumanInventory
        gPatterns.C_HumanInventory__AddItem                       = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 41 8B D6");
        gPatterns.C_HumanInventory__AddItemByData                 = hook::get_opcode_address("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 4B 18 45 33 C9");
        gPatterns.C_HumanInventory__AddMedkits                    = hook::get_opcode_address("E8 ? ? ? ? 48 8B 8B ? ? ? ? 49 BF ? ? ? ? ? ? ? ");
        gPatterns.C_HumanInventory__AddWeapon                     = hook::get_opcode_address("E8 ? ? ? ? 84 C0 74 44 48 8B 4B 18");
        gPatterns.C_HumanInventory__CanAddAmmoByCategoryAux       = hook::get_opcode_address("E8 ? ? ? ? 85 C0 C6 84 24 ? ? ? ? ?");
        gPatterns.C_HumanInventory__CanAddAmmoByCategoryAuxAndMag = hook::get_opcode_address("E8 ? ? ? ? 85 C0 7E 20");
        gPatterns.C_HumanInventory__CanAddUpgrade                 = hook::get_opcode_address("E8 ? ? ? ? 40 0A F0");
        gPatterns.C_HumanInventory__CanAddWeapon                  = hook::get_opcode_address("E8 ? ? ? ? 80 7C 24 ? ? 75 12");
        gPatterns.C_HumanInventory__CanFire                       = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 2E F6 83 ? ? ? ? ?");
        gPatterns.C_HumanInventory__CanReload                     = hook::get_opcode_address("E8 ? ? ? ? 3C 01 75 06");
        gPatterns.C_HumanInventory__CanUseMedkit                  = hook::get_opcode_address("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 93 ? ? ? ?");
        gPatterns.C_HumanInventory__CreateDefaultItems            = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 48 89 6C 24 ? 57 41 56 41 57 48 83 EC 50 48 8B F9").get_first());
        gPatterns.C_HumanInventory__CreateModel                   = hook::get_opcode_address("E8 ? ? ? ? 4C 8D B7 ? ? ? ? 48 8B F0");
        gPatterns.C_HumanInventory__DestroyModel                  = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 57 48 83 EC 40 48 83 B9 ? ? ? ? ? 0F B6 FA").get_first());
        gPatterns.C_HumanInventory__DoReload                      = hook::get_opcode_address("E8 ? ? ? ? 49 8B D4 48 8D 8E ? ? ? ?");
        gPatterns.C_HumanInventory__DoShot                        = hook::get_opcode_address("E8 ? ? ? ? F7 87 ? ? ? ? ? ? ? ? 74 1B");
        gPatterns.C_HumanInventory__DuplicateWeaponModel          = hook::get_opcode_address("E8 ? ? ? ? 48 8B D8 48 8D 45 97");
        gPatterns.C_HumanInventory__GetSelectedAmmoCategory       = hook::get_opcode_address("E8 ? ? ? ? 3B C3 74 04");
        gPatterns.C_HumanInventory__SelectAnimSetting             = hook::get_opcode_address("E8 ? ? ? ? 84 C0 74 B6 B0 01");
        gPatterns.C_HumanInventory__SelectByItemID                = hook::get_opcode_address("E8 ? ? ? ? 48 8B 87 ? ? ? ? 48 8D 55 20");
        gPatterns.C_HumanInventory__SelectNextWeapon              = reinterpret_cast<uint64_t>(hook::pattern("40 56 48 81 EC ? ? ? ? 48 8B 41 78").get_first());
        gPatterns.C_HumanInventory__TellMedkit                    = hook::get_opcode_address("E8 ? ? ? ? 3B C7 7D 0C");
        gPatterns.C_HumanInventory__UseMedkit                     = hook::get_opcode_address("E8 ? ? ? ? 0F B6 D8 84 C0 74 28");

        // C_HumanWeaponController
        gPatterns.C_HumanWeaponController__DoWeaponSelectByItemId = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 40 48 8B 81 60"));
        gPatterns.C_HumanWeaponController__GetRightHandWeaponID   = hook::get_opcode_address("E8 ? ? ? ? 3B 46 78 ");
        gPatterns.C_HumanWeaponController__IsThrownWeapon         = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 81 ? ? ? ? 8B 40 48"));
        gPatterns.C_HumanWeaponController__GetShotPosDir          = hook::get_opcode_address("E8 ? ? ? ? F2 0F 10 44 24 ? 48 8B CF");
        gPatterns.C_HumanWeaponController__ResetScatterCoef       = reinterpret_cast<uint64_t>(hook::get_pattern("40 57 48 83 EC 30 48 8B F9 0F 29 74 24 ?"));
        gPatterns.C_HumanWeaponController__SetAiming              = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 10 48 8B 88 ? ? ? ? C6 41 14 00");
        gPatterns.C_HumanWeaponController__SetCoverFlag           = hook::get_opcode_address("E8 ? ? ? ? 48 8D 97 ? ? ? ? 49 8B CE");
        gPatterns.C_HumanWeaponController__SetFirePressedFlag     = reinterpret_cast<uint64_t>(hook::get_pattern("84 D2 75 11 33 C0"));
        gPatterns.C_HumanWeaponController__SetStickMove           = reinterpret_cast<uint64_t>(hook::get_pattern("F2 0F 10 02 F2 0F 11 81 ? ? ? ? C3"));
        gPatterns.C_HumanWeaponController__SetZoomFlag            = hook::get_opcode_address("E8 ? ? ? ? F6 87 ? ? ? ? ? 49 8B CE");
        gPatterns.C_HumanWeaponController__DoShot                 = hook::get_opcode_address("E8 ? ? ? ? 0F B6 D8 84 DB 0F 84 ? ? ? ?");
        gPatterns.C_HumanWeaponController__DoWeaponReloadShowMagazine = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4F 08 4C 8B C6 BA ? ? ? ? E8 ? ? ? ? 45 33 F6");
        gPatterns.C_HumanWeaponController__DoWeaponReloadDropMagazine = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 56 57 48 83 EC 60 B8 ? ? ? ?"));
        gPatterns.C_HumanWeaponController__DoWeaponReloadInventory = reinterpret_cast<uint64_t>(hook::get_pattern("33 C0 44 8B C2 48 89 81 ? ? ? ?"));

        // C_Human2
        gPatterns.C_Human2__EnableHumanClothes = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 8B 7C 24 ? 48 83 C4 40 41 5E C3 CC CC 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54");
        gPatterns.C_Human2__EnableShadows      = hook::get_opcode_address("E8 ? ? ? ? 80 7F 18 09 75 56");

        // C_Navigation
        gPatterns.C_Navigation__GetInstance           = hook::get_opcode_address("E8 ? ? ? ? 49 8B 5E 60");
        gPatterns.C_Navigation__SetUserMark           = hook::get_opcode_address("E8 ? ? ? ? 48 83 C4 ? C3 ? ? ? ? ? ? ? 48 89 5C 24 ? 57 48 83 EC ? 41 8B D8");
        gPatterns.C_Navigation__RegisterVehicleCommon = hook::get_opcode_address("E8 ? ? ? ? 89 87 ? ? ? ? 48 8B CF E8 ? ? ? ?");
        gPatterns.C_Navigation__RegisterVehicleEntity = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F0 48 8B DA 48 8B E9"));
        gPatterns.C_Navigation__UnregisterVehicle     = hook::get_opcode_address("E8 ? ? ? ? 44 38 B6 ? ? ? ? 74 1B");

        // init
        gPatterns.C_InitDone_MafiaFrameworkAddr = reinterpret_cast<uint64_t>(hook::pattern("40 55 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 57 C0").get_first());
        gPatterns.LoadIntroAddr                 = reinterpret_cast<uint64_t>(hook::pattern("48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8B E9 B9 ? ? ? ?").get_first());

        // C_CommandLine
        gPatterns.C_CommandLine__FindCommandAddr = hook::get_opcode_address("E8 ? ? ? ? 40 88 7D BB");

        // C_GameFramework
        gPatterns.C_GameFramework__IsSuspendedAddr = reinterpret_cast<uint64_t>(hook::pattern("80 39 ? 74 ? 80 79 ? ?").get_first());

        // C_GameTrafficModule
        gPatterns.C_GameTrafficModule__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 80 7E 07 00");

        // C_Fader
        gPatterns.C_Fader__FadeIn = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4D 67 48 8B 9C 24 ? ? ? ?");
        gPatterns.C_Fader__FadeOut = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 0F 28 74 24 ? 48 85 C9 74 1E");

        // Lua
        gPatterns.Lua__pcallAddr      = hook::get_opcode_address("E8 ? ? ? ? 85 FF 78 2B");
        gPatterns.Lua__loadbufferAddr = hook::get_opcode_address("E8 ? ? ? ? 85 C0 74 2F 48 8B 47 48");
        gPatterns.Lua__tostringAddr   = reinterpret_cast<uint64_t>(hook::pattern("4C 8B C9 81 FA ? ? ? ? 7E 37").get_first());
        gPatterns.Lua__isstringAddr   = hook::get_opcode_address("E8 ? ? ? ? 85 C0 74 5E 8B D3");
    }
}; // namespace SDK
