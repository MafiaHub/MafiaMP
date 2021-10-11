#include "entity_factory.h"

#include <utility>

namespace MafiaMP::Game::Streaming {
    static SDK::ue::game::traffic::C_HumanSpawner *CreateHumanSpawner(const SDK::ue::sys::utils::C_HashName &spawnProfile) {
        return new SDK::ue::game::traffic::C_HumanSpawner(spawnProfile, 200);
    }

    static EntitySpawnerState HumanSpawnerProcessLoad(SDK::ue::game::traffic::C_HumanSpawner *spawner) {
        return (spawner->IsSpawnProfileLoaded() ? EntitySpawnerState::Ready : EntitySpawnerState::Loading);
    }

    static std::pair<EntitySpawnResult, SDK::C_Entity *> SpawnHuman(SDK::ue::game::traffic::C_HumanSpawner *spawner) {
        SDK::C_Actor *actor = nullptr;
        SDK::ue::C_WeakPtr<SDK::ue::sys::core::C_SceneObject> sceneObject;
        spawner->CreateActor(sceneObject, actor);
        return actor ? std::pair(EntitySpawnResult::OK, actor) : std::pair(EntitySpawnResult::Failed, nullptr);
    }

    static void ReturnHumanToSpawner(SDK::ue::game::traffic::C_HumanSpawner *spawner, SDK::C_Entity *entity) {
        spawner->ReturnObject(entity->GetSceneObject());
    }

    static void DestroyHumanSpawner(SDK::ue::game::traffic::C_HumanSpawner *spawner) {
        delete spawner;
    }

    static SDK::mafia::streaming::C_ActorsSlotWrapper *CreateVehicleSpawner(const std::pair<SDK::E_EntityType, std::string> &vehicleKind) {
        auto slotWrapper = new SDK::mafia::streaming::C_ActorsSlotWrapper(vehicleKind.first, 1, 0, vehicleKind.second.c_str(), true);
        if (slotWrapper) {
            bool unk = true;
            slotWrapper->ConnectToQuota("Misc", SDK::streammap::flags::E_CAR_SMALL, _I32_MAX);
            if (slotWrapper->LoadData(std::string("/sds/cars/" + vehicleKind.second + ".sds").c_str(), nullptr, 2, "MafiaMP_Vehicle", &unk, true)) {
                return slotWrapper;
            }
            else {
                slotWrapper->DisconnectFromQuota();
                delete slotWrapper;
            }
        }
        return nullptr;
    }

    static EntitySpawnerState VehicleSpawnerProcessLoad(SDK::mafia::streaming::C_ActorsSlotWrapper *slotWrapper) {
        switch (slotWrapper->GetActorsState()) {
        case SDK::mafia::streaming::C_ActorsSlotWrapper::E_ASS_CREATING: {
            SDK::C_TimeBudgetInfo::C_Ctx TimeBudgetInfoCtx;
            TimeBudgetInfoCtx.ForceMinBudget(0.001f);
            TimeBudgetInfoCtx.BeginUpdate();
            slotWrapper->Update(TimeBudgetInfoCtx);
            TimeBudgetInfoCtx.EndUpdate();
            return EntitySpawnerState::Loading;
        }
        case SDK::mafia::streaming::C_ActorsSlotWrapper::E_ASS_READY: {
            return EntitySpawnerState::Ready;
        }
        default: return EntitySpawnerState::Loading;
        }
    }

    static std::pair<EntitySpawnResult, SDK::C_Entity *> SpawnVehicle(SDK::mafia::streaming::C_ActorsSlotWrapper *slotWrapper) {
        if (slotWrapper->GetNumFree() <= 0) {
            if (slotWrapper->GetActorsState() == SDK::mafia::streaming::C_ActorsSlotWrapper::E_ASS_READY) {
                slotWrapper->UpdateToCreateActors(slotWrapper->GetToCreateActorsWithoutCrewCnt() + 1, 1);
            }

            return std::pair(EntitySpawnResult::KeepLoading, nullptr);
        }

        SDK::C_Actor *vehicle = nullptr;
        SDK::ue::C_WeakPtr<SDK::ue::sys::core::C_SceneObject> sceneObject;
        slotWrapper->GetFreeActor(sceneObject, false, &vehicle);
        return vehicle ? std::pair(EntitySpawnResult::OK, vehicle) : std::pair(EntitySpawnResult::Failed, nullptr);
    }

    static void ReturnVehicleToSpawner(SDK::mafia::streaming::C_ActorsSlotWrapper *slotWrapper, SDK::C_Entity *vehicle) {
        slotWrapper->ReturnActor((SDK::C_Actor *)vehicle);
    }

    static void DestroyVehicleSpawner(SDK::mafia::streaming::C_ActorsSlotWrapper *slotWrapper) {
        slotWrapper->Close(true);
        delete slotWrapper;
    }

    EntityFactory::EntityFactory()
        : _humanFactory(&CreateHumanSpawner, &HumanSpawnerProcessLoad, &SpawnHuman, &ReturnHumanToSpawner, &DestroyHumanSpawner)
        , _vehicleFactory(&CreateVehicleSpawner, &VehicleSpawnerProcessLoad, &SpawnVehicle, &ReturnVehicleToSpawner, &DestroyVehicleSpawner) {}

    void EntityFactory::Update() {
        _humanFactory.Update();
        _vehicleFactory.Update();
    }

    EntityTrackingInfo *EntityFactory::RequestHuman(SDK::ue::sys::utils::C_HashName spawnProfile) {
        return _humanFactory.Request(SDK::E_EntityType::E_ENTITY_HUMAN, spawnProfile);
    }

    EntityTrackingInfo *EntityFactory::RequestVehicle(SDK::E_EntityType type, const std::string &modelName) {
        return _vehicleFactory.Request(type, std::make_pair(type, modelName));
    }

    void EntityFactory::ReturnEntity(EntityTrackingInfo *infos) {
        switch (infos->GetType()) {
        case SDK::E_EntityType::E_ENTITY_HUMAN: {
            _humanFactory.Return(infos);
            break;
        }
        case SDK::E_EntityType::E_ENTITY_CAR:
        case SDK::E_EntityType::E_ENTITY_BOAT: {
            _vehicleFactory.Return(infos);
            break;
        }
        }
    }

    void EntityFactory::ReturnAll() {
        _vehicleFactory.ReturnAll();
        _humanFactory.ReturnAll();
    }
} // namespace MafiaMP::Game::Streaming