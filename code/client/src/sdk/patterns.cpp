#include "patterns.h"

namespace SDK {
    Patterns gPatterns;

    void Patterns::InitPatterns() {
        // C_ActorsSlotWrapper
        gPatterns.C_ActorsSlotWrapper__C_ActorsSlotWrapper  = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 74 24 ? 48 8D 05 ? ? ? ? 33 ED"));
        gPatterns.C_ActorsSlotWrapper__Close                = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 55 48 83 EC ? C7 41 ? ? ? ? ?"));
        gPatterns.C_ActorsSlotWrapper__GetFreeActor         = reinterpret_cast<uint64_t>(hook::pattern("41 56 48 83 EC ? 33 C0 48 8B F9").get_first(-0xA));
        gPatterns.C_ActorsSlotWrapper__ReturnActor          = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 56 48 83 EC ? 48 8B 41 ? 40 32 ED"));
        gPatterns.C_ActorsSlotWrapper__UpdateToCreateActors = hook::get_opcode_address("E8 ? ? ? ? EB 25 83 FF 0A");

        // C_BehaviorCharacter
        gPatterns.C_BehaviorCharacter__SetWAnimVariable_float = hook::get_opcode_address("E9 ? ? ? ? 83 FA 20");

        // C_Car
        gPatterns.C_Car__CloseHood           = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B 81 ? ? ? ? 33 DB 48 2B 81 ? ? ? ? 33 FF 48 C1 F8 ? 48 8B F1 4C 63 F0 85 C0 0F 8E ? ? ? ? 48 89 6C 24 ? 0F 29 74 24 ? 0F 57 F6 66 66 0F 1F 84 00 ? ? ? ? 48 8B 86 ? ? ? ? 48 63 0C B8 48 8B 86 ? ? ? ? 48 8B 14 C8 33 C9 8B 42 ? 39 05 ? ? ? ? 48 0F 45 CA 48 85 C9 74 ? 48 0F BF 49 ? 48 8B 86 ? ? ? ? 48 8B 14 C8 0F 2F 72 ? 73 ? 0F B6 86 ? ? ? ? 48 8D 8E ? ? ? ? C0 E8 ? 45 33 C0"));
        gPatterns.C_Car__CloseTrunk          = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4F ? 4C 8D 47 ? BA ? ? ? ? E8 ? ? ? ? 84 C0 0F 84 ? ? ? ?");
        gPatterns.C_Car__ExplodeCar          = hook::get_opcode_address("E8 ? ? ? ? 48 8B 86 ? ? ? ? 45 33 E4 48 2B 86 ? ? ? ?");
        gPatterns.C_Car__ExplodeCar_2        = reinterpret_cast<uint64_t>(hook::get_pattern("F3 0F 11 4C 24 ? 4C 8B DC 55"));
        gPatterns.C_Car__GetDamage           = hook::get_opcode_address("E8 ? ? ? ? 0F 57 C9 0F 2E C1 75 ? 0F 28 C1");
        gPatterns.C_Car__GetFuelTankCapacity = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC 28 48 8B 81 ? ? ? ? 48 8B 48 08 48 8B 09 48 85 C9 74 1E"));
        gPatterns.C_Car__GetMotorDamage      = hook::get_opcode_address("E8 ? ? ? ? 49 8B CE 0F 28 F8 E8 ? ? ? ? 0F 28 F0");
        gPatterns.C_Car__Lock                = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? B8 ? ? ? ? C3");
        gPatterns.C_Car__LockEntryPoints     = hook::get_opcode_address("E8 ? ? ? ? 45 8D 7E ? 48 8B 4F ?");

        //NOTE: new version only
#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Car__OpenHood =
            hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 40 53 48 83 EC ? 45 33 C0 33 D2 48 8B D9 E8 ? ? ? ? 48 8B 43 ? C7 40 ? ? ? ? ? C7 00 ? ? ? ? 48 83 C0 ? 48 89 43 ? B8 ? ? ? ? 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? 48 83 EC ?");
#endif
        gPatterns.C_Car__OpenTrunk = hook::get_opcode_address("E8 ? ? ? ? 4D 85 F6 74 ? 49 8D 9E ? ? ? ?");

#ifndef NONSTEAM_SUPPORT
        // TODO lol
        gPatterns.C_Car__RestoreCar = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4B 10 0F 57 C9");
#else
        gPatterns.C_Car__RestoreCar = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4B 10 0F 57 C9");
#endif
        gPatterns.C_Car__SetActualFuel   = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 81 ? ? ? ? 48 8B 48 08 48 8B 09 48 85 C9 0F 85 ? ? ? ? C3 CC CC CC CC CC CC CC CC 0F 28 C1"));
        gPatterns.C_Car__SetMotorDamage  = hook::get_opcode_address("E8 ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 49 21 86 ? ? ? ?");
        gPatterns.C_Car__SetSeatStatus   = hook::get_opcode_address("E8 ? ? ? ? 45 85 F6 8B C5");
        gPatterns.C_Car__SetSpeed        = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 33 C0 48 83 C4 ? 5F C3 ? ? 48 83 EC ? 48 8B C2");
        gPatterns.C_Car__SetTransparency = hook::get_opcode_address("E8 ? ? ? ? 4C 39 2D ? ? ? ?");

#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Car__SetVehicleDirty = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? 4C 8B 41 ?");
#else
        gPatterns.C_Car__SetVehicleDirty = hook::get_opcode_address("E8 ? ? ? ? 8B 55 60 83 FA FF");
#endif
        gPatterns.C_Car__Unlock            = hook::get_opcode_address("E8 ? ? ? ? C6 43 ? ? 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? ? C2 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? C2 ? ?");
        gPatterns.C_Car__UnlockEntryPoints = hook::get_opcode_address("E8 ? ? ? ? 48 8B 6C 24 ? 48 83 C4 ? 5E C3 ? ? ? ? ? ? ? ? ? 48 89 5C 24 ?");

        // C_CharacterController
        gPatterns.C_CharacterController__ActivateHandler                   = hook::get_opcode_address("E8 ? ? ? ? 48 8D 56 74");
        gPatterns.C_CharacterController__DeactivateHandler_FromPlayerInput = hook::get_opcode_address("E8 ? ? ? ? EB 0F 48 8B 93 ? ? ? ?");
        gPatterns.C_CharacterController__TriggerActorActionById            = hook::get_opcode_address("E8 ? ? ? ? 45 0F 2F 87 ? ? ? ?");

        // C_CharacterStateHandlerAim
        gPatterns.C_CharacterStateHandlerAim__SwappingWeapon     = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 82");
        gPatterns.C_CharacterStateHandlerAim__UpdateAimAnimation = hook::get_opcode_address("E8 ? ? ? ? F3 0F 10 43 ? 48 8D 55 07");

        // C_CharacterStateHandlerBaseLocomotion
        gPatterns.C_CharacterStateHandlerBaseLocomotion__Idle2MoveTransitionActive = hook::get_opcode_address("E8 ? ? ? ? 48 89 75 E0 C7 45 ? ? ? ? ?");
        gPatterns.C_CharacterStateHandlerBaseLocomotion__AddRemoveSprintDescriptor = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 08 45 33 FF");

        // C_CharacterStateHandlerMove
        gPatterns.C_CharacterStateHandlerMove__SharpTurnTransitionActive = hook::get_opcode_address("E8 ? ? ? ? 84 C0 74 60 40 84 FF");

        // C_CommandLine
        gPatterns.C_CommandLine__FindCommand = hook::get_opcode_address("E8 ? ? ? ? 40 88 7D BB");

        // C_Ctx
        gPatterns.C_Ctx__BeginUpdate = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 48 89 51 ? 48 8D 4C 24 ?"));
        gPatterns.C_Ctx__EndUpdate   = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 83 79 ? ? 48 8B D9 74 ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D 4C 24 ?"));

        // C_DatabaseSystem
        gPatterns.C_DatabaseSystem__GetDatabase =
            reinterpret_cast<uint64_t>(hook::get_pattern("4C 8B 51 08 4D 8B C2 49 8B 42 08 80 78 19 00 75 1B 4C 8B 0A 4C 39 48 20 73 06 48 8B 40 10 EB 06 4C 8B C0 48 8B 00 80 78 19 00 74 E8 4D 3B C2 74 09 49 8B 40 20 48 39 02 73 03 4D 8B C2 4D 3B C2 74 05"));

        // C_Door
        gPatterns.C_Door__AILock        = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? F6 81 ? ? ? ? ? 48 8B D9 75 ? 83 B9 ? ? ? ? ? 74 ? E8 ? ? ? ? 48 8B C8 41 B0 ? 48 8B D3 E8 ? ? ? ? 83 8B ? ? ? ? ?"));
        gPatterns.C_Door__AIUnlock      = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? F6 81 ? ? ? ? ? 48 8B D9 75 ? 83 B9 ? ? ? ? ? 74 ? E8 ? ? ? ? 48 8B C8 41 B0 ? 48 8B D3 E8 ? ? ? ? 83 A3 ? ? ? ? ?"));
        gPatterns.C_Door__Close         = hook::get_opcode_address("E8 ? ? ? ? C6 86 ? ? ? ? ? 33 D2");
        gPatterns.C_Door__DisableAction = reinterpret_cast<uint64_t>(hook::get_pattern("83 A1 ? ? ? ? ? 48 8B 81 ? ? ? ? 48 85 C0"));
        gPatterns.C_Door__EnableAction  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 33 C0 48 83 C4 ? C3 ? ? ? ? ? ? ? ? ? ? ? ? ? 48 83 EC ?");
        gPatterns.C_Door__Kick          = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 48 8B 42 ? 8B 48 ?");
        gPatterns.C_Door__Lock          = reinterpret_cast<uint64_t>(hook::get_pattern("F6 81 ? ? ? ? ? 48 8B D1 75 ? 83 B9 ? ? ? ? ? 74 ? 8B 89 ? ? ? ? 85 C9"));
        gPatterns.C_Door__Open          = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 83 B9 ? ? ? ? ? 0F 85 ? ? ? ?");
        gPatterns.C_Door__StartLockpick = reinterpret_cast<uint64_t>(hook::get_pattern("F6 81 ? ? ? ? ? 75 ? 83 B9 ? ? ? ? ? 74 ? 48 81 C1 ? ? ? ? E9 ? ? ? ?"));
        gPatterns.C_Door__StopLockpick  = reinterpret_cast<uint64_t>(hook::get_pattern("F6 81 ? ? ? ? ? 75 ? 83 B9 ? ? ? ? ? 74 ? 83 B9 ? ? ? ? ? 75 ? 45 33 C0"));
        gPatterns.C_Door__ToggleOpen    = hook::get_opcode_address("E8 ? ? ? ? EB ? 80 7A ? ? 74 ?");
        gPatterns.C_Door__Unlock        = hook::get_opcode_address("E8 ? ? ? ? EB ? 83 F8 ? 75 ? 48 8B 83 ? ? ? ?");

        // C_Entity
        gPatterns.C_Entity__Activate   = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8B D9 8B 49 ? 8B C1 25 ? ? ? ?"));
        gPatterns.C_Entity__Deactivate = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 8B 41 ? 48 8B D9 25 ? ? ? ? 3D ? ? ? ?"));
        gPatterns.C_Entity__GameDone   = reinterpret_cast<uint64_t>(hook::get_pattern("40 57 48 83 EC ? 8B 41 ? 48 8B F9 25 ? ? ? ?"));
        gPatterns.C_Entity__GameInit   = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8B D9 8B 49 ? 8B C1 C1 E8 ? A8 ?"));
        gPatterns.C_Entity__Release    = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 8B 41 ? 48 8B D9 25 ? ? ? ?"));

        // C_EntityFactory
        gPatterns.C_EntityFactory__ComputeHash    = reinterpret_cast<uint64_t>(hook::get_pattern("0F B6 11 33 C0 84 D2 74 2C"));
        gPatterns.C_EntityFactory__CreateEntity   = hook::get_opcode_address("E8 ? ? ? ? 48 89 46 10 48 8B 4E 10");
        gPatterns.C_EntityFactory__RegisterEntity = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC 58 4C 8B C1"));

        // C_EntityList
        gPatterns.C_EntityList__GetEntityList = hook::get_opcode_address("E8 ? ? ? ? 8B 53 40 48 8B C8");

        // C_Explosion
        gPatterns.C_Explosion__Clear = hook::get_opcode_address("E8 ? ? ? ? 66 03 DD 75 E5");

        // C_Fader
        gPatterns.C_Fader__FadeIn  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4D 67 48 8B 9C 24 ? ? ? ?");
        gPatterns.C_Fader__FadeOut = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 0F 28 74 24 ? 48 85 C9 74 1E");
        gPatterns.C_Fader__Reset   = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC 38 80 B9 ? ? ? ? ? 74 34"));

        // C_Fire
        gPatterns.C_Fire__Clear = hook::get_opcode_address("E8 ? ? ? ? 66 03 DD 75 E7");

        // C_Game
        gPatterns.C_Game__GetGame = hook::get_opcode_address("E8 ? ? ? ? 0F B7 5B 20");

        // C_GameAudioModule
        gPatterns.C_GameAudioModule__GetAudioModule    = hook::get_opcode_address("E8 ? ? ? ? 48 8B C8 8D 53 51");
        gPatterns.C_GameAudioModule__SetCutsceneVolume = hook::get_opcode_address("E9 ? ? ? ? CC CC CC 48 8B 81 ? ? ? ? 89 90 ? ? ? ?");
        gPatterns.C_GameAudioModule__SetDialogueVolume = hook::get_opcode_address("E9 ? ? ? ? CC CC CC 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 49 8B F0 48 8B F9");
        gPatterns.C_GameAudioModule__SetDynamicRange   = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC 20 8B DA 89 91 ? ? ? ? E8 ? ? ? ?"));
        gPatterns.C_GameAudioModule__SetMasterVolume   = hook::get_opcode_address("E9 ? ? ? ? CC CC CC 48 8B 49 20 E9 ? ? ? ?");
        gPatterns.C_GameAudioModule__SetMusicVolume    = hook::get_opcode_address("E9 ? ? ? ? CC CC CC 48 89 5C 24 ? 57 48 83 EC 70 48 8B 59 70");
        gPatterns.C_GameAudioModule__SetSfxVolume      = hook::get_opcode_address("E9 ? ? ? ? CC CC CC 48 8B C4 48 81 EC ? ? ? ? F3 0F 10 15 ? ? ? ? 0F 57 C0");

        // C_GameCamera
        gPatterns.C_GameCamera__GetInstanceInternal = hook::get_opcode_address("E8 ? ? ? ? 48 8B C8 4C 8D 4D 30");

        // C_GameDirector
        gPatterns.C_GameDirector__GetDistrict = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 50 F2 0F 10 02"));

        // C_GameFramework
        gPatterns.C_GameFramework__IsSuspended = reinterpret_cast<uint64_t>(hook::get_pattern("80 39 ? 74 ? 80 79 ? ?"));

        // C_GameGfxEnvEffModule
        gPatterns.C_GameGfxEnvEffModule__GetCurrentWeatherSetName = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 05 ? ? ? ? 48 8B 40 28 48 8B 40 28"));

        // C_GameGUI2Module
        uint64_t C_GameGUI2Module       = hook::get_opcode_address("E8 ? ? ? ? 41 8D 56 11");
        uint8_t *C_GameGUI2Module_Bytes = reinterpret_cast<uint8_t *>(C_GameGUI2Module);

        gPatterns.C_GameGUI2Module__GetDatabase                 = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D B9 ? ? ? ? 48 8B F2"));
        gPatterns.C_GameGUI2Module__GetGameGui2Module           = hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 40 80 F6 01");
        gPatterns.C_GameGUI2Module__Instance                    = reinterpret_cast<uint64_t>(C_GameGUI2Module_Bytes + *(int32_t *)(C_GameGUI2Module_Bytes + 3) + 7);
        gPatterns.C_GameGUI2Module__SendHUDSimpleBooleanMessage = hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 49 8B 97 ? ? ? ? 4C 8D 05 ? ? ? ?");
        gPatterns.C_GameGUI2Module__SendMessageMovie            = hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 48 8D 5D 58");

        // C_GameInputModule
        gPatterns.C_GameInputModule__GetGameInputModule = hook::get_opcode_address("E8 ? ? ? ? 4C 8B F0 8B D7");
        gPatterns.C_GameInputModule__PauseInput         = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 08 C6 80 ? ? ? ? ? 48 8B 5C 24 ?");

        // C_GameTrafficModule
        gPatterns.C_GameTrafficModule__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 80 7E 07 00");

        // C_GameRenderingSystemModule
        gPatterns.C_GameRenderingSystemModule__GetGameRenderingSystemModule = hook::get_opcode_address("E8 ? ? ? ? 45 33 C0 0F B6 D3");

        // C_GfxEnvironmentEffects
        uint64_t C_GfxEnvironmentEffects       = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 05 ? ? ? ? 48 8B 40 28 48 8B 40 28"));
        uint8_t *C_GfxEnvironmentEffects_Bytes = reinterpret_cast<uint8_t *>(C_GfxEnvironmentEffects);

        gPatterns.C_GfxEnvironmentEffects__Instance = reinterpret_cast<uint64_t>(C_GfxEnvironmentEffects_Bytes + *(int32_t *)(C_GfxEnvironmentEffects_Bytes + 3) + 7);

        // C_HashName
        gPatterns.C_HashName__ComputeHash = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 74 24 ? 41 56 48 83 EC ? 4C 8B F2 48 8B F1 48 85 C9"));
        gPatterns.C_HashName__SetName     = reinterpret_cast<uint64_t>(hook::get_pattern("44 0F B6 11 4C 8B C2"));

        // C_Human2
        gPatterns.C_Human2__EnableHumanClothes = hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 8B 7C 24 ? 48 83 C4 40 41 5E C3 CC CC 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54");
        gPatterns.C_Human2__EnableShadows      = hook::get_opcode_address("E8 ? ? ? ? 80 7F 18 09 75 56");

        // C_Human2CarWrapper
        gPatterns.C_Human2CarWrapper__GetSeatID = hook::get_opcode_address("E8 ? ? ? ? 3D ? ? ? ? 75 0E");
        gPatterns.C_Human2CarWrapper__IsEngineOn = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 16 44 8B 87 ? ? ? ?");

        // C_HumanInventory
        gPatterns.C_HumanInventory__AddItem                       = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 41 8B D6");
        gPatterns.C_HumanInventory__AddItemByData                 = hook::get_opcode_address("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 4B 18 45 33 C9");
        gPatterns.C_HumanInventory__AddMedkits                    = hook::get_opcode_address("E8 ? ? ? ? 48 8B 8B ? ? ? ? 49 BF ? ? ? ? ? ? ? ");
        gPatterns.C_HumanInventory__AddMoney                      = reinterpret_cast <uint64_t>(hook::get_pattern("48 01 91 ? ? ? ? B0"));
        gPatterns.C_HumanInventory__AddWeapon                     = hook::get_opcode_address("E8 ? ? ? ? 84 C0 74 44 48 8B 4B 18");
        gPatterns.C_HumanInventory__CanAddAmmoByCategoryAux       = hook::get_opcode_address("E8 ? ? ? ? 85 C0 C6 84 24 ? ? ? ? ?");
        gPatterns.C_HumanInventory__CanAddAmmoByCategoryAuxAndMag = hook::get_opcode_address("E8 ? ? ? ? 85 C0 7E 20");
        gPatterns.C_HumanInventory__CanAddUpgrade                 = hook::get_opcode_address("E8 ? ? ? ? 40 0A F0");
        gPatterns.C_HumanInventory__CanAddWeapon                  = hook::get_opcode_address("E8 ? ? ? ? 80 7C 24 ? ? 75 12");
        gPatterns.C_HumanInventory__CanFire                       = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 2E F6 83 ? ? ? ? ?");
        gPatterns.C_HumanInventory__CanReload                     = hook::get_opcode_address("E8 ? ? ? ? 3C 01 75 06");
        gPatterns.C_HumanInventory__CanUseMedkit                  = hook::get_opcode_address("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 93 ? ? ? ?");
        gPatterns.C_HumanInventory__CreateDefaultItems            = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 57 41 56 41 57 48 83 EC 50 48 8B F9"));
        gPatterns.C_HumanInventory__CreateModel                   = hook::get_opcode_address("E8 ? ? ? ? 4C 8D B7 ? ? ? ? 48 8B F0");
        gPatterns.C_HumanInventory__DestroyModel                  = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC 40 48 83 B9 ? ? ? ? ? 0F B6 FA"));
        gPatterns.C_HumanInventory__DoReload                      = hook::get_opcode_address("E8 ? ? ? ? 49 8B D4 48 8D 8E ? ? ? ?");
        gPatterns.C_HumanInventory__DoShot                        = hook::get_opcode_address("E8 ? ? ? ? F7 87 ? ? ? ? ? ? ? ? 74 1B");
        gPatterns.C_HumanInventory__DuplicateWeaponModel          = hook::get_opcode_address("E8 ? ? ? ? 48 8B D8 48 8D 45 97");
        gPatterns.C_HumanInventory__GetSelectedAmmoCategory       = hook::get_opcode_address("E8 ? ? ? ? 3B C3 74 04");
        gPatterns.C_HumanInventory__SelectAnimSetting             = hook::get_opcode_address("E8 ? ? ? ? 84 C0 74 B6 B0 01");
        gPatterns.C_HumanInventory__SelectByItemID                = hook::get_opcode_address("E8 ? ? ? ? 48 8B 87 ? ? ? ? 48 8D 55 20");
        gPatterns.C_HumanInventory__SelectNextWeapon              = reinterpret_cast<uint64_t>(hook::get_pattern("40 56 48 81 EC ? ? ? ? 48 8B 41 78"));
        gPatterns.C_HumanInventory__TellMedkit                    = hook::get_opcode_address("E8 ? ? ? ? 3B C7 7D 0C");
        gPatterns.C_HumanInventory__UseMedkit                     = hook::get_opcode_address("E8 ? ? ? ? 0F B6 D8 84 C0 74 28");

        // C_HumanScript
        gPatterns.C_HumanScript__GetOffVehicle  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? 83 69 ? ? 75 ? 48 8B 01 FF 50 ? 48 8B 8B ? ? ? ? 4C 8D 05 ? ? ? ?");
        gPatterns.C_HumanScript__GetOnVehicle   = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? 48 8B 5C 24 ? 48 85 C9 74 ? 83 69 ? ? 75 ? 48 8B 01 FF 50 ? 48 8B 06");
        gPatterns.C_HumanScript__ScrAim         = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 20 48 8B DA 48 8B F9 48 8B 0D ? ? ? ? 48 8D 55 20 41 0F B6 F0"));
        gPatterns.C_HumanScript__ScrAimAt       = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8B EC 48 83 EC 30 48 8B DA"));
        gPatterns.C_HumanScript__ScrAttack      = reinterpret_cast<uint64_t>(hook::get_pattern("48 85 D2 0F 84 ? ? ? ? 55 56 41 56"));
        gPatterns.C_HumanScript__SetHealth      = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC ? 48 8B 09 0F 29 74 24 ?"));
        gPatterns.C_HumanScript__SetStealthMove = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC 60 48 8B F9 0F B6 DA"));

        // C_HumanSpawner
        gPatterns.C_HumanSpawner__C_HumanSpawnerVtbl          = hook::get_opcode_address("49 8B CE E8 ? ? ? ? 48 8B 5C 24 ? 48 8D 05 ? ? ? ?", 15);
        gPatterns.C_HumanSpawner__SetupDefaultArchetypeObject = reinterpret_cast<uint64_t>(hook::get_pattern("4C 8B DC 49 89 5B ? 56 57 41 56 48 83 EC ? 48 8B 1D ? ? ? ?"));

        // C_HumanWeaponController
        gPatterns.C_HumanWeaponController__DoShot                     = hook::get_opcode_address("E8 ? ? ? ? 0F B6 D8 84 DB 0F 84 ? ? ? ?");
        gPatterns.C_HumanWeaponController__DoWeaponReloadDropMagazine = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 56 57 48 83 EC 60 B8 ? ? ? ?"));
        gPatterns.C_HumanWeaponController__DoWeaponReloadInventory    = reinterpret_cast<uint64_t>(hook::get_pattern("33 C0 44 8B C2 48 89 81 ? ? ? ?"));
        gPatterns.C_HumanWeaponController__DoWeaponReloadShowMagazine = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4F 08 4C 8B C6 BA ? ? ? ? E8 ? ? ? ? 45 33 F6");
        gPatterns.C_HumanWeaponController__DoWeaponSelectByItemId     = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 40 48 8B 81 60"));
        gPatterns.C_HumanWeaponController__GetRightHandWeaponID       = hook::get_opcode_address("E8 ? ? ? ? 3B 46 78 ");
        gPatterns.C_HumanWeaponController__GetShotPosDir              = hook::get_opcode_address("E8 ? ? ? ? F2 0F 10 44 24 ? 48 8B CF");
        gPatterns.C_HumanWeaponController__IsThrownWeapon             = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 81 ? ? ? ? 8B 40 48"));
        gPatterns.C_HumanWeaponController__ResetScatterCoef           = reinterpret_cast<uint64_t>(hook::get_pattern("40 57 48 83 EC 30 48 8B F9 0F 29 74 24 ?"));
        gPatterns.C_HumanWeaponController__SetAiming                  = hook::get_opcode_address("E8 ? ? ? ? 48 8B 43 10 48 8B 88 ? ? ? ? C6 41 14 00");
        gPatterns.C_HumanWeaponController__SetCoverFlag               = hook::get_opcode_address("E8 ? ? ? ? 48 8D 97 ? ? ? ? 49 8B CE");
        gPatterns.C_HumanWeaponController__SetFirePressedFlag         = reinterpret_cast<uint64_t>(hook::get_pattern("84 D2 75 11 33 C0"));
        gPatterns.C_HumanWeaponController__SetStickMove               = reinterpret_cast<uint64_t>(hook::get_pattern("F2 0F 10 02 F2 0F 11 81 ? ? ? ? C3"));
        gPatterns.C_HumanWeaponController__SetZoomFlag                = hook::get_opcode_address("E8 ? ? ? ? F6 87 ? ? ? ? ? 49 8B CE");

        // C_IE
        gPatterns.C_IE__Alloc = hook::get_opcode_address("48 ? ? 48 ? ? ? ? B9 04 14 00 00 E8 ? ? ? ? 48", 0xD);
        gPatterns.C_IE__Free  = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 41 B8 0D 00 00 00 48 ? ? ? ? ? ? 48");

        // C_InventoryWrapper
        gPatterns.C_InventoryWrapper__AddMoney  = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC 20 48 8B 41 68 48 8B DA 80 78 18 09 75 4E"));
        gPatterns.C_InventoryWrapper__AddWeapon = hook::get_opcode_address("E8 ? ? ? ? 41 38 76 4C 74 0C");
        gPatterns.C_InventoryWrapper__TellMoney = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC 28 48 8B 41 68 80 78 18 09"));

        // C_MafiaDBs
        gPatterns.C_MafiaDBs__GetMafiaDBs         = hook::get_opcode_address("E8 ? ? ? ? 48 8D 55 87 48 8B C8");
        gPatterns.C_MafiaDBs__GetTablesDatabase   = hook::get_opcode_address("E8 ? ? ? ? 48 8B F8 49 8B C7");
        gPatterns.C_MafiaDBs__GetVehiclesDatabase = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC 20 48 8B DA 41 B8 06 00 00 00"));

        // C_MafiaFramework
        // NOTE: extract address from another function and its instruction
        uint64_t C_MafiaFramework       = hook::get_opcode_address("E8 ? ? ? ? E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 33 D2 48 8B C8") + 0xD;
        uint8_t *C_MafiaFramework_Bytes = reinterpret_cast<uint8_t *>(C_MafiaFramework);

        gPatterns.C_MafiaFramework__Instance = reinterpret_cast<uint64_t>(C_MafiaFramework_Bytes + *(int32_t *)(C_MafiaFramework_Bytes + 3) + 7);

        // C_Matrix
        gPatterns.C_Matrix__SetDir      = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 89 43 ? 89 43 ? 89 43 ? 48 83 C4 ? 5B C3 ? 40 53 48 83 EC ? 48 8B D9 48 8D 4C 24 ?"));
        gPatterns.C_Matrix__SetDir2     = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? F2 0F 10 02 48 8B D9 8B 42 ?"));
        gPatterns.C_Matrix__SetDir3     = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? F3 0F 10 02 0F 57 DB"));
        gPatterns.C_Matrix__SetRot      = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 89 43 ? 89 43 ? 89 43 ? 48 83 C4 ? 5B C3 ? 40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ?"));
        gPatterns.C_Matrix__SetRotEuler = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 33 C0 89 43 ? 89 43 ? 89 43 ? 48 83 C4 ? 5B C3 ? 0F 10 02"));

        // C_MenuSave
        gPatterns.C_MenuSave__OpenDebugLoadChapterString = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B F9 4C 8B F2"));

        // C_Motor
#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Motor__SetFuel = hook::get_opcode_address("E8 ? ? ? ? 33 C0 0F 28 74 24 ? 48 83 C4 ? 5B C3 ? ? ? 48 89 5C 24 ?");
#else
        gPatterns.C_Motor__SetFuel = hook::get_opcode_address("E8 ? ? ? ? 41 0F 10 96 ? ? ? ?");
#endif

        // C_Navigation
        gPatterns.C_Navigation__EnableGPSCustomPath       = hook::get_opcode_address("E8 ? ? ? ? 4D 85 F6 0F 84 ? ? ? ? 4D 2B E6");
        gPatterns.C_Navigation__GetInstance               = hook::get_opcode_address("E8 ? ? ? ? 49 8B 5E 60");
        gPatterns.C_Navigation__RegisterHumanPlayer       = hook::get_opcode_address("E8 ? ? ? ? 48 83 C6 10 49 3B F7");
        gPatterns.C_Navigation__RegisterHumanPolice       = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC 38 45 33 C9 C6 44 24 ? ?"));
        gPatterns.C_Navigation__RegisterVehicleCommon     = hook::get_opcode_address("E8 ? ? ? ? 89 87 ? ? ? ? 48 8B CF E8 ? ? ? ?");
        gPatterns.C_Navigation__RegisterVehicleEntity     = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F0 48 8B DA 48 8B E9"));
        gPatterns.C_Navigation__RegisterVehicleMoto       = hook::get_opcode_address("E8 ? ? ? ? 48 8B 74 24 ? 48 8B 5C 24 ? 48 83 C4 20 5F C3 8B 48 04");
        gPatterns.C_Navigation__RegisterVehiclePolice     = hook::get_opcode_address("E8 ? ? ? ? 48 81 8E ? ? ? ? ? ? ? ? 89 86 ? ? ? ?");
        gPatterns.C_Navigation__RegisterVehiclePoliceBoat = hook::get_opcode_address("E8 ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 09 87 ? ? ? ? 48 8B 5C 24 ?");
        gPatterns.C_Navigation__RegisterVehiclePoliceMoto = hook::get_opcode_address("E8 ? ? ? ? 89 86 ? ? ? ? C6 86 ? ? ? ? ? 48 8B 5C 24 ?");
        gPatterns.C_Navigation__RegisterVehicleTaxi       = hook::get_opcode_address("E8 ? ? ? ? 48 89 77 38 33 F6");
        gPatterns.C_Navigation__SetUserMark               = hook::get_opcode_address("E8 ? ? ? ? 48 83 C4 ? C3 ? ? ? ? ? ? ? 48 89 5C 24 ? 57 48 83 EC ? 41 8B D8");
        gPatterns.C_Navigation__UnregisterHuman           = hook::get_opcode_address("E8 ? ? ? ? 8B 05 ? ? ? ? 48 8B 7C 24 ?");
        gPatterns.C_Navigation__UnregisterId              = hook::get_opcode_address("E8 ? ? ? ? 39 BE ? ? ? ? 75 0A");
        gPatterns.C_Navigation__UnregisterVehicle         = hook::get_opcode_address("E8 ? ? ? ? 44 38 B6 ? ? ? ? 74 1B");

        // C_PlayerModelManager
        gPatterns.C_PlayerModelManager__IsPlayerLoaded      = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 49 28 48 85 C9 75 03"));
        gPatterns.C_PlayerModelManager__SwitchPlayerProfile = hook::get_opcode_address("E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 41 48");

        // C_PlayerTeleportModule
        uint64_t C_PlayerTeleportModule       = hook::get_opcode_address("E8 ? ? ? ? 83 78 38 00 74 13") + 0xD;
        uint8_t *C_PlayerTeleportModule_Bytes = reinterpret_cast<uint8_t *>(C_PlayerTeleportModule);

        gPatterns.C_PlayerTeleportModule__GetPlayerTeleportModule = hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 83 78 38 00 74 13");
        gPatterns.C_PlayerTeleportModule__Instance                = reinterpret_cast<uint64_t>(C_PlayerTeleportModule_Bytes + *(int32_t *)(C_PlayerTeleportModule_Bytes + 3) + 7);
        gPatterns.C_PlayerTeleportModule__TeleportPlayer          = reinterpret_cast<uint64_t>(hook::get_pattern("40 56 48 83 EC 50 83 79 38 00"));

        // C_ProfileSpawner
        gPatterns.C_ProfileSpawner__C_ProfileSpawner      = hook::get_opcode_address("49 8B CE E8 ? ? ? ? 48 8B 5C 24 ? 48 8D 05 ? ? ? ?", 3);
        gPatterns.C_ProfileSpawner__C_ProfileSpawnerDctor = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 56 57 48 83 EC ? 48 8D 05 ? ? ? ? 48 8B D9"));
        gPatterns.C_ProfileSpawner__CreateActor           = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4C 24 ? BA ? ? ? ? 48 89 4B 08");
        gPatterns.C_ProfileSpawner__IsSpawnProfileLoaded  = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B C8 48 8D 54 24 ? E8 ? ? ? ? 4C 8D 83 ? ? ? ?"));
        gPatterns.C_ProfileSpawner__ReturnObject          = hook::get_opcode_address("E8 ? ? ? ? EB 6F 48 8B 5F 08");

        // C_Quat
        gPatterns.C_Quat__SetDir = hook::get_opcode_address("E8 ? ? ? ? F3 44 0F 59 5D ?");
        
        // C_RaceTimer
        gPatterns.C_RaceTimer_SetVisible = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 0F B6 F2"));
        gPatterns.C_RaceTimer_StartRace  = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 C7 41 ? ? ? ? ? 41 0F B7 C9"));

        // C_SceneObject
        gPatterns.C_SceneObject__SetTransform = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 8D 41 ? 48 8B D9 0F 10 02"));

        // C_ShotManager
        gPatterns.C_ShotManager__CreateExplosion = hook::get_opcode_address("E8 ? ? ? ? C6 83 ? ? ? ? ? 48 85 F6");
        gPatterns.C_ShotManager__CreateFire      = hook::get_opcode_address("E8 ? ? ? ? 48 8D 4D 88 89 87 ? ? ? ?");
        gPatterns.C_ShotManager__GetInstance     = hook::get_opcode_address("E8 ? ? ? ? 49 8B 4D 18 48 8B F8");

        // C_SlotWrapper
        gPatterns.C_SlotWrapper__LoadData = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 41 ? 41 8B F9 49 8B F0 48 8B EA"));

        // C_StreamingModule
        gPatterns.C_StreamingModule__SetStreamingPosSource = reinterpret_cast<uint64_t>(hook::get_pattern("33 C0 89 91 ? ? ? ? 48 89 81 ? ? ? ?"));

        // C_StreamingTrafficModule
        gPatterns.C_StreamingTrafficModule__CloseSeason         = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 6C 24 ? 41 56 48 83 EC 20 83 B9 ? ? ? ? ? 44 0F B6 F2"));
        gPatterns.C_StreamingTrafficModule__GetSeasonOpened     = hook::get_opcode_address("E8 ? ? ? ? 33 D2 84 C0 74 0B ");
        gPatterns.C_StreamingTrafficModule__GetInstance         = hook::get_opcode_address("E8 ? ? ? ? 4C 8B 44 F3 ?");
        gPatterns.C_StreamingTrafficModule__OpenSeason          = hook::get_opcode_address("E8 ? ? ? ? 48 8B 76 08 48 8B CE");
        gPatterns.C_StreamingTrafficModule__SetMaxHumanElements = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC 28 3B 91 ? ? ? ? 74 25"));

        // C_StreamMap
        gPatterns.C_StreamMap__CloseGame    = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 30 48 8B A9 ? ? ? ? 4C 8B F1 48 C7 44 24 ? ? ? ? ? B9 ? ? ? ? C7 44 24 ? ? ? ? ? 48 8B 75 08 E8 ? ? ? ? 48 8B D8 48 85 C0 75 07 FF 15 ? ? ? ? CC 48 89 7C 24 ? 48 8D 78 08 48 89 28 48 85 FF 74 03 48 89 37 48 8D 48 10 48 85 C9 74 34 8B 44 24 20 48 8D 71 08 89 01 48 8D 54 24 ? 48 8B 44 24 ? 48 8B CE 48 89 06 E8 ? ? ? ? 84 C0 75 11 48 83 3E 00 74 0B 48 8B CE E8 ? ? ? ? F0 FF 00 49 8B 8E ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 2B C1 48 83 F8 01 73 0E 48 8D 0D ? ? ? ? FF 15 ? ? ? ? CC 48 8D 41 01 49 89 86 ? ? ? ? 48 89 5D 08 48 8B 07 48 89 18 48 83 7C 24 ? ? 74 1E 48 8D 4C 24 ? E8 ? ? ? ? 83 C9 FF F0 0F C1 08 83 F9 01 75 08 48 8B C8 E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 30 41 5E C3 CC CC CC CC 40 53 48 83 EC 20 44 8B 41 10 48 8D 81 ? ? ? ? 48 89 81 ? ? ? ? 32 DB 41 8D 40 FF"));
        gPatterns.C_StreamMap__CloseMission = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 30 48 8B A9 ? ? ? ? 4C 8B F1 48 C7 44 24 ? ? ? ? ? B9 ? ? ? ? C7 44 24 ? ? ? ? ? 48 8B 75 08 E8 ? ? ? ? 48 8B D8 48 85 C0 75 07 FF 15 ? ? ? ? CC 48 89 7C 24 ? 48 8D 78 08 48 89 28 48 85 FF 74 03 48 89 37 48 8D 48 10 48 85 C9 74 34 8B 44 24 20 48 8D 71 08 89 01 48 8D 54 24 ? 48 8B 44 24 ? 48 8B CE 48 89 06 E8 ? ? ? ? 84 C0 75 11 48 83 3E 00 74 0B 48 8B CE E8 ? ? ? ? F0 FF 00 49 8B 8E ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 2B C1 48 83 F8 01 73 0E 48 8D 0D ? ? ? ? FF 15 ? ? ? ? CC 48 8D 41 01 49 89 86 ? ? ? ? 48 89 5D 08 48 8B 07 48 89 18 48 83 7C 24 ? ? 74 1E 48 8D 4C 24 ? E8 ? ? ? ? 83 C9 FF F0 0F C1 08 83 F9 01 75 08 48 8B C8 E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 30 41 5E C3 CC CC CC CC 48 89 5C 24 ?"));
        gPatterns.C_StreamMap__ClosePart    = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 30 48 8B A9 ? ? ? ? 4C 8B F1 48 C7 44 24 ? ? ? ? ? B9 ? ? ? ? C7 44 24 ? ? ? ? ? 48 8B 75 08 E8 ? ? ? ? 48 8B D8 48 85 C0 75 07 FF 15 ? ? ? ? CC 48 89 7C 24 ? 48 8D 78 08 48 89 28 48 85 FF 74 03 48 89 37 48 8D 48 10 48 85 C9 74 34 8B 44 24 20 48 8D 71 08 89 01 48 8D 54 24 ? 48 8B 44 24 ? 48 8B CE 48 89 06 E8 ? ? ? ? 84 C0 75 11 48 83 3E 00 74 0B 48 8B CE E8 ? ? ? ? F0 FF 00 49 8B 8E ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 2B C1 48 83 F8 01 73 0E 48 8D 0D ? ? ? ? FF 15 ? ? ? ? CC 48 8D 41 01 49 89 86 ? ? ? ? 48 89 5D 08 48 8B 07 48 89 18 48 83 7C 24 ? ? 74 1E 48 8D 4C 24 ? E8 ? ? ? ? 83 C9 FF F0 0F C1 08 83 F9 01 75 08 48 8B C8 E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 30 41 5E C3 CC CC CC CC 40 53 48 83 EC 20 44 8B 41 10 48 8D 81 ? ? ? ? 48 89 81 ? ? ? ? 32 DB 41 8D 40 FD"));
        gPatterns.C_StreamMap__GetGame      = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 41 10 48 85 C0 48 0F 44 05 ? ? ? ? C3 48 8B 81 ? ? ? ?"));
        gPatterns.C_StreamMap__GetMission   = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 41 18 48 85 C0 48 0F 44 05 ? ? ? ? C3 48 8B 41 20"));
        gPatterns.C_StreamMap__GetPart      = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 41 20 48 85 C0 48 0F 44 05 ? ? ? ? C3 40 53"));
        gPatterns.C_StreamMap__OpenGame     = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 30 33 C0"));
        gPatterns.C_StreamMap__OpenMission  = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 30 4C 8B F1 48 C7 44 24 ? ? ? ? ? 48 8D 4C 24 ? C7 44 24 ? ? ? ? ? E8 ? ? ? ? 49 8B AE ? ? ? ? B9 ? ? ? ? 48 8B 75 08 E8 ? ? ? ? 48 8B D8 48 85 C0 75 07 FF 15 ? ? ? ? CC 48 89 7C 24 ? 48 8D 78 08 48 89 28 48 85 FF 74 03 48 89 37 48 8D 70 10 48 85 F6 74 34 8B 44 24 20 48 8D 54 24 ? 89 06 48 8D 4E 08 48 8B 44 24 ? 48 89 46 08 E8 ? ? ? ? 84 C0 75 13 48 83 7E ? ? 74 0C 48 8D 4E 08 E8 ? ? ? ? F0 FF 00 49 8B 8E ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 2B C1 48 83 F8 01 73 0E 48 8D 0D ? ? ? ? FF 15 ? ? ? ? CC 48 8D 41 01 49 89 86 ? ? ? ? 48 89 5D 08 48 8B 07 48 89 18 48 83 7C 24 ? ? 74 1E 48 8D 4C 24 ? E8 ? ? ? ? 83 C9 FF F0 0F C1 08 83 F9 01 75 08 48 8B C8 E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 30 41 5E C3 CC CC CC CC CC CC CC CC CC CC 40 53"));
        gPatterns.C_StreamMap__OpenPart     = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 30 4C 8B F1 48 C7 44 24 ? ? ? ? ? 48 8D 4C 24 ? C7 44 24 ? ? ? ? ? E8 ? ? ? ? 49 8B AE ? ? ? ? B9 ? ? ? ? 48 8B 75 08 E8 ? ? ? ? 48 8B D8 48 85 C0 75 07 FF 15 ? ? ? ? CC 48 89 7C 24 ? 48 8D 78 08 48 89 28 48 85 FF 74 03 48 89 37 48 8D 70 10 48 85 F6 74 34 8B 44 24 20 48 8D 54 24 ? 89 06 48 8D 4E 08 48 8B 44 24 ? 48 89 46 08 E8 ? ? ? ? 84 C0 75 13 48 83 7E ? ? 74 0C 48 8D 4E 08 E8 ? ? ? ? F0 FF 00 49 8B 8E ? ? ? ? 48 B8 ? ? ? ? ? ? ? ? 48 2B C1 48 83 F8 01 73 0E 48 8D 0D ? ? ? ? FF 15 ? ? ? ? CC 48 8D 41 01 49 89 86 ? ? ? ? 48 89 5D 08 48 8B 07 48 89 18 48 83 7C 24 ? ? 74 1E 48 8D 4C 24 ? E8 ? ? ? ? 83 C9 FF F0 0F C1 08 83 F9 01 75 08 48 8B C8 E8 ? ? ? ? 48 8B 7C 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 74 24 ? 48 83 C4 30 41 5E C3 CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ?"));

        // C_String
        gPatterns.C_String__SetString = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B F1 48 85 D2 75 ? 48 8B 09"));

        // C_SysODB
        gPatterns.C_SysODB__GetInstance = hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 41 B8 6F 50 30 4C");

        // C_TickedModuleManager
        gPatterns.C_TickedModuleManager__GetTickedModuleManager = hook::get_opcode_address("E8 ? ? ? ? 45 8B 46 24");

        // C_TrafficSpawnManager
        gPatterns.C_TrafficSpawnManager__GetCivilCarDensityMult = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC 20 45 33 C0 33 D2 48 8B D9 E8 ? ? ? ? 48 8D 0D ? ? ? ?"));
        gPatterns.C_TrafficSpawnManager__Populate             = hook::get_opcode_address("E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 89 03");
        gPatterns.C_TrafficSpawnManager__SetTrainDensity      = reinterpret_cast<uint64_t>(hook::get_pattern("74 0C 48 8B 81 ? ? ? ? F3 0F 11 48 ?"));
        gPatterns.C_TrafficSpawnManager__SwitchAmbientTraffic = reinterpret_cast<uint64_t>(hook::get_pattern("0F 85 ? ? ? ? 38 91 ? ? ? ? 0F 84 ? ? ? ? 48 89 5C 24 ?"));
        gPatterns.C_TrafficSpawnManager__UpdateMaxElementsCnt = hook::get_opcode_address("E9 ? ? ? ? 48 83 C4 28 C3 CC CC CC 88 51 50");

        // C_Translocator
        gPatterns.C_Translocator__SpawnObject = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B C4 48 89 58 ? 48 89 70 ? 48 89 78 ? 4C 89 70 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B DA 48 8B 02"));
        gPatterns.C_Translocator__SpawnObjects = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 48 8B FA 48 8B CA E8 ? ? ? ? 84 C0 0F 85"));

        // C_TranslocableObjectSpawner
        gPatterns.C_TranslocableObjectSpawner__Spawn = reinterpret_cast<uint64_t>(hook::get_pattern("44 88 4C 24 ? 89 54 24"));

        // C_Vehicle
        gPatterns.C_Vehicle__AddVehicleFlags    = hook::get_opcode_address("E8 ? ? ? ? 40 F6 C7 08 74 0D");
        gPatterns.C_Vehicle__ChangeRadioStation = reinterpret_cast<uint64_t>(hook::get_pattern("40 57 48 83 EC 20 48 83 B9 90 12 00 00 00"));
        gPatterns.C_Vehicle__ClearVehicleFlags  = hook::get_opcode_address("E8 ? ? ? ? 40 80 FE 04");

        //NOTE: new version only
#ifndef NONSTEAM_SUPPORT
        gPatterns.C_Vehicle__Damage = hook::get_opcode_address("E8 ? ? ? ? 33 C0 48 8B 5C 24 ? 48 83 C4 ? 5F C3 ? ? ? ? ? ? B8 ? ? ? ?");
#else
        // TODO
#endif

        gPatterns.C_Vehicle__DamageBreaks      = reinterpret_cast<uint64_t>(hook::get_pattern(
            "40 53 48 83 EC ? 48 8B 01 48 8B D9 0F 29 7C 24 ? 0F 28 F9 FF 50 ? 84 C0 74 ? 48 8B 5B ? 4C 8D 44 24 ? 0F 29 74 24 ? 48 8D 54 24 ? 0F 57 F6 C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? 48 8D 8B ? ? ? ? E8 ? ? ? ? F3 0F 10 0D ? ? ? ? 0F 28 C7 F3 0F 5C C1 0F 2F C6 73 ? 0F 28 CF 0F 28 C1 0F 57 05 ? ? ? ? 0F 2F C6 73 ? 0F 28 F1 F3 0F 59 35 ? ? ? ? 48 8D 8B ? ? ? ? F3 0F 10 54 24 ?"));
        gPatterns.C_Vehicle__EnableRadio       = hook::get_opcode_address("E8 ? ? ? ? 49 8B 84 24 ? ? ? ? 49 8B F7");
        gPatterns.C_Vehicle__GetSPZText        = hook::get_opcode_address("E8 ? ? ? ? 49 8D 4F ? 48 8B D0");
        gPatterns.C_Vehicle__IsActive          = hook::get_opcode_address("E8 ? ? ? ? 84 C0 75 0A B2 01");
        gPatterns.C_Vehicle__IsAnyLightOn      = reinterpret_cast<uint64_t>(hook::get_pattern("48 8B 81 ? ? ? ? 48 8B 89 ? ? ? ? 48 3B C1 74 18 48 8B 10"));
        gPatterns.C_Vehicle__IsSiren           = hook::get_opcode_address("E8 ? ? ? ? 0F B6 4D BF");
        gPatterns.C_Vehicle__SetActive         = hook::get_opcode_address("E8 ? ? ? ? F3 0F 59 35 ? ? ? ? 48 8D 8B ? ? ? ?");
        gPatterns.C_Vehicle__SetAngularSpeed   = hook::get_opcode_address("E8 ? ? ? ? E9 ? ? ? ? 80 BD ? ? ? ? ? 0F 85 ? ? ? ?");
        gPatterns.C_Vehicle__SetBeaconLightsOn = hook::get_opcode_address("E8 ? ? ? ? 48 8B 7C 24 ? 48 83 C4 ? 5B C3 48 8B D9");
        gPatterns.C_Vehicle__SetBrake          = hook::get_opcode_address("E8 ? ? ? ? C7 87 ? ? ? ? ? ? ? ? 83 A7 ? ? ? ? ?");
        gPatterns.C_Vehicle__SetDoorFree =
            hook::get_opcode_address("E8 ? ? ? ? 48 8B 5C 24 ? 33 C0 48 8B 74 24 ? 48 83 C4 ? 5F C3 ? ? ? 40 53 48 83 EC ? 45 33 C0 33 D2 48 8B D9 E8 ? ? ? ? 48 8B 43 ? C7 40 ? ? ? ? ? C7 00 ? ? ? ? 48 83 C0 ? 48 89 43 ? B8 ? ? ? ? 48 83 C4 ? 5B C3 ? ? ? ? ? ? ? ? ? B8 ? ? ? ?");
        gPatterns.C_Vehicle__SetEngineOn        = hook::get_opcode_address("E8 ? ? ? ? 48 8B 9E ? ? ? ? 0F B6 C3");
        gPatterns.C_Vehicle__SetGear            = hook::get_opcode_address("E8 ? ? ? ? C6 83 ? ? ? ? ? 80 7F ? ? 74 ? 33 D2");
        gPatterns.C_Vehicle__SetHandbrake       = hook::get_opcode_address("E8 ? ? ? ? 49 8B 86 ? ? ? ? 4D 89 BE ? ? ? ?");
        gPatterns.C_Vehicle__SetHorn            = hook::get_opcode_address("E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B F0");
        gPatterns.C_Vehicle__SetIndicatorLightsOn = reinterpret_cast<uint64_t>(hook::get_pattern("B8 ? ? ? ? 45 84 C0 41 B9"));
        gPatterns.C_Vehicle__SetInteriorColors  = hook::get_opcode_address("E8 ? ? ? ? 48 81 C4 ? ? ? ? 41 5E 41 5D 5D C3 ? ? ? ? ? ? ? ? ? ? 40 55");
        gPatterns.C_Vehicle__SetPower           = hook::get_opcode_address("E8 ? ? ? ? F3 0F 10 8B ? ? ? ? 45 33 C0 48 8B CF E8 ? ? ? ?");
        gPatterns.C_Vehicle__SetReflectorLightsOn = reinterpret_cast<uint64_t>(hook::get_pattern("48 81 C1 ? ? ? ? E9 ? ? ? ? CC CC CC CC 48 83 EC ? 4C 8B C9"));
        gPatterns.C_Vehicle__SetSearchLightsOn  = hook::get_opcode_address("E8 ? ? ? ? 80 7E ? ? 0F 84 ? ? ? ? E8 ? ? ? ?");
        gPatterns.C_Vehicle__SetSiren           = hook::get_opcode_address("E8 ? ? ? ? 33 D2 48 8B CE E8 ? ? ? ? 48 8B 0D ? ? ? ?");
        gPatterns.C_Vehicle__SetSpeed           = hook::get_opcode_address("E8 ? ? ? ? 49 8B CF E8 ? ? ? ? 45 0F 57 C9");
        gPatterns.C_Vehicle__SetSpeedLimit      = hook::get_opcode_address("E8 ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9 74 ? 48 8B 89 ? ? ? ?");
        gPatterns.C_Vehicle__SetSPZText         = hook::get_opcode_address("E8 ? ? ? ? 48 8D 5D ? BF ? ? ? ? 0F B7 13");
        gPatterns.C_Vehicle__SetSteer           = hook::get_opcode_address("E8 ? ? ? ? 41 B1 01 89 BE ? ? ? ?");
        gPatterns.C_Vehicle__SetTaxiLightsOn    = reinterpret_cast<uint64_t>(hook::get_pattern("81 A1 ? ? ? ? ? ? ? ? 0F B6 C2 F7 D8 25 ? ? ? ? 09 81 ? ? ? ? C3 CC CC CC CC CC F3 0F 11 89"));
        gPatterns.C_Vehicle__SetVehicleColor    = hook::get_opcode_address("E8 ? ? ? ? 8B 43 ? 89 87 ? ? ? ? 8B 43 ? 89 87 ? ? ? ? 83 7B ? ?");
        gPatterns.C_Vehicle__SetVehicleDirty    = hook::get_opcode_address("E9 ? ? ? ? ? ? ? ? ? ? ? ? ? ? 40 57 48 83 EC ? 48 8B 81 ? ? ? ?");
        gPatterns.C_Vehicle__SetVehicleMatrix   = hook::get_opcode_address("E8 ? ? ? ? 80 BD ? ? ? ? ? 74 0B");
        gPatterns.C_Vehicle__SetVehicleRust     = hook::get_opcode_address("E9 ? ? ? ? ? ? ? ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56");
        gPatterns.C_Vehicle__SetWheelColor      = hook::get_opcode_address("E8 ? ? ? ? 8B 43 4C 89 87 ? ? ? ?");
        gPatterns.C_Vehicle__SetWindowTintColor = hook::get_opcode_address("E8 ? ? ? ? 89 AE ? ? ? ? 4C 8D 9C 24 ? ? ? ?");
        gPatterns.C_Vehicle__TurnRadioOn        = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 B9 ? ? ? ? ? 44 0F B6 CA"));

        // C_VehiclesDatabase
        gPatterns.C_VehiclesDatabase__GetVehicleByID    = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B 41 30 33 FF 41 8B D8 48 8B F2 48 8B E9 39 78 18 76 79"));
        gPatterns.C_VehiclesDatabase__GetVehicleByIndex = reinterpret_cast<uint64_t>(hook::get_pattern(
            "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 20 48 8B 41 30 33 DB 41 8B F8 4C 8B F2 48 8B F1 39 58 18 76 34 0F 1F 80 00 00 00 00 8B D3 48 8B C8 E8 ? ? ? ? 33 D2 48 8B C8 4C 8B 00 41 FF 50 ? 4C 8B C0 8B 48 04 3B F9 72 2D 48 8B 46 30 2B F9 FF C3 3B 58 18 72 D3 49 C7 06 00 00 00 00 48 8B 5C 24 30 49 8B C6 48 8B 74 24 38 48 8B 7C 24 40 48 83 C4 20 41 5E C3 49 63 10 8B C7 48 69 C0 F0 00 00 00"));
        gPatterns.C_VehiclesDatabase__GetVehicleByModel = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 55 41 56 41 57 48 83 EC 20 48 8B 41"));
        gPatterns.C_VehiclesDatabase__GetVehiclesCount  = hook::get_opcode_address<uint64_t>("E8 ? ? ? ? 48 8B 4D 30 8B F8");

        // C_VehicleRealLightManager
        gPatterns.C_VehicleRealLightManager__SetLightIntensity = reinterpret_cast<uint64_t>(hook::get_pattern("40 53 48 83 EC ? 48 63 C2 0F 29 74 24"));
        gPatterns.C_VehicleRealLightManager__SetPlayerLights   = reinterpret_cast<uint64_t>(hook::get_pattern("4C 8B DC 55 56 41 57 48 83 EC"));
        gPatterns.C_VehicleRealLightManager__SetReflectorLightsOn = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC ? 4C 8B C9 84 D2 74 ? 45 84 C0"));
        gPatterns.C_VehicleRealLightManager__SetVisible           = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 57 48 83 EC ? 8B 41 ? 0F B6 FA D1 E8"));

        // C_WAnimPlaybackManager
        gPatterns.C_WAnimPlaybackManager__PlayState = hook::get_opcode_address("E8 ? ? ? ? 4C 39 7F 50");

        // C_WeatherManager2
        gPatterns.C_WeatherManager2__EnableTimeFlow              = reinterpret_cast<uint64_t>(hook::get_pattern("88 91 ? ? ? ? C6 81 ? ? ? ? ? C6 81 ? ? ? ? ? C3"));
        gPatterns.C_WeatherManager2__GetDayTimeHours             = hook::get_opcode_address("E8 ? ? ? ? 48 8B 4B 38 33 C0");
        gPatterns.C_WeatherManager2__GetDayTimeRel               = hook::get_opcode_address("E8 ? ? ? ? F3 0F 11 43 ? 48 8D 54 24 ?");
        gPatterns.C_WeatherManager2__GetDefaultTimeFlowSpeedMult = reinterpret_cast<uint64_t>(hook::get_pattern("F3 0F 10 81 ? ? ? ? C3 CC CC CC CC CC CC CC 8B 05 ? ? ? ?"));
        gPatterns.C_WeatherManager2__GetUserTimeFlowSpeedMult    = reinterpret_cast<uint64_t>(hook::get_pattern("F3 0F 10 81 ? ? ? ? C3 CC CC CC CC CC CC CC 41 83 F8 04"));
        gPatterns.C_WeatherManager2__IsTimeFlowEnabled           = reinterpret_cast<uint64_t>(hook::get_pattern(
            "0F B6 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 32 C0 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC B0 01"));
        gPatterns.C_WeatherManager2__SetDayTimeHours             = hook::get_opcode_address("E8 ? ? ? ? 48 83 C4 30 5F 5E 5B C3 CC CC CC CC CC CC CC CC CC 48 83 EC 38");
        gPatterns.C_WeatherManager2__SetDayTimeSec               = hook::get_opcode_address("E9 ? ? ? ? 8B 47 18 89 44 24 40");
        gPatterns.C_WeatherManager2__SetDefaultTimeFlowSpeedMult = reinterpret_cast<uint64_t>(hook::get_pattern("F3 0F 11 89 ? ? ? ? C3 CC CC CC CC CC CC CC F3 0F 11 89 ? ? ? ? C3 CC CC CC CC CC CC CC F3 0F 59 0D ? ? ? ?"));
        gPatterns.C_WeatherManager2__SetUserTimeFlowSpeedMult    = reinterpret_cast<uint64_t>(hook::get_pattern("F3 0F 11 89 ? ? ? ? C3 CC CC CC CC CC CC CC 83 FA 04"));
        gPatterns.C_WeatherManager2__SetWeatherSet               = hook::get_opcode_address("E8 ? ? ? ? 48 83 7C 24 ? ? 0F 28 74 24 ?");

        // I_Core
        gPatterns.I_Core__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 4C 8B 40 68");

        // I_GameDirector
        gPatterns.I_GameDirector__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 48 8B 57 18 84 DB");

        // I_VirtualFileSystemCache
        gPatterns.I_VirtualFileSystemCache__GetInstance = hook::get_opcode_address("E8 ? ? ? ? 41 0F B7 CF");

        // Lua
        gPatterns.Lua__isstring   = hook::get_opcode_address("E8 ? ? ? ? 85 C0 74 5E 8B D3");
        gPatterns.Lua__loadbuffer = hook::get_opcode_address("E8 ? ? ? ? 85 C0 74 2F 48 8B 47 48");
        gPatterns.Lua__pcall      = hook::get_opcode_address("E8 ? ? ? ? 85 FF 78 2B");
        gPatterns.Lua__tostring   = reinterpret_cast<uint64_t>(hook::get_pattern("4C 8B C9 81 FA ? ? ? ? 7E 37"));

        // init
        gPatterns.C_InitDone_MafiaFramework = reinterpret_cast<uint64_t>(hook::get_pattern("40 55 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 57 C0"));
        gPatterns.LoadIntro                 = reinterpret_cast<uint64_t>(hook::get_pattern("48 89 5C 24 ? 55 56 57 48 83 EC ? 48 8B E9 B9 ? ? ? ?"));

        // TODO: rename me
        gPatterns.renameme__SpawnObject  = hook::get_opcode_address("E8 ? ? ? ? 33 F6 EB 9F");
        gPatterns.renameme__SpawnObject2 = hook::get_opcode_address("E8 ? ? ? ? 49 8B 4C 3F ?");
        gPatterns.renameme__SpawnObject3 = hook::get_opcode_address("E8 ? ? ? ? 4C 8B E8 49 8B CD");

        // script
        gPatterns.Script__GetWeaponIdByName = reinterpret_cast<uint64_t>(hook::get_pattern("48 83 EC ? 48 85 C9 0F 84 ? ? ? ? 80 39"));
    }
}; // namespace SDK
