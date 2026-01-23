#include "entity_browser.h"

#include "core/application.h"

#include <sdk/entities/c_car.h>
#include <sdk/entities/c_entity_list.h>
#include <sdk/entities/c_human_2.h>
#include <sdk/entities/c_player_2.h>
#include <sdk/prefab/c_prefab_manager.h>
#include <sdk/ue/sys/core/c_scene_object.h>
#include <sdk/ue/sys/core/i_component.h>

#include "core/modules/human.h"
#include "core/modules/vehicle.h"

#include "game/helpers/controls.h"

#include <logging/logger.h>
#include <set>

namespace MafiaMP::Core::UI::Devs {
    static const char *GetPrefabTypeName(SDK::prefab::E_PREFAB_TYPE type) {
        switch (type) {
        case SDK::prefab::E_PT_Base: return "Base";
        case SDK::prefab::E_PT_Car: return "Car";
        case SDK::prefab::E_PT_CrashObj: return "CrashObj";
        case SDK::prefab::E_PT_ActorDeform: return "ActorDeform";
        case SDK::prefab::E_PT_Wheel: return "Wheel";
        case SDK::prefab::E_PT_Door: return "Door";
        case SDK::prefab::E_PT_Obsolete_Lift: return "Obsolete_Lift";
        case SDK::prefab::E_PT_Boat: return "Boat";
        case SDK::prefab::E_PT_Obsolete_Wagon: return "Obsolete_Wagon";
        case SDK::prefab::E_PT_AIBrain: return "AIBrain";
        case SDK::prefab::E_PT_MountedWeapon: return "MountedWeapon";
        case SDK::prefab::E_PT_TrafficSemaphore: return "TrafficSemaphore";
        case SDK::prefab::E_PT_None: return "None";
        default: return "Unknown";
        }
    }

    EntityBrowser::EntityBrowser(): UIBase() {
        InitialiseEntityTypes();
    }

    void EntityBrowser::OnOpen() {}

    void EntityBrowser::OnClose() {}

    void EntityBrowser::OnUpdate() {
        auto entityList = SDK::GetEntityList();
        if (!entityList) {
            return;
        }

        auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!localPlayer) {
            return;
        }

        auto windowContent = [&]() {
            if (ImGui::Button("Select all")) {
                for (size_t i = 0; i < (sizeof(_checkedTypes) / sizeof(_checkedTypes[0])); i++) _checkedTypes[i] = true;

                for (auto &[entityType, entityName] : _allTypes) {
                    if (std::find(_filterList.begin(), _filterList.end(), entityType) == _filterList.end())
                        _filterList.push_back(entityType);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Deselect all")) {
                for (size_t i = 0; i < 66; i++) _checkedTypes[i] = false;

                _filterList.clear();
            }

            ImGui::SameLine();

            if (ImGui::Button("Dump to Console")) {
                auto logger = Framework::Logging::GetLogger("EntityBrowser");
                std::set<std::string> uniqueNames;
                const auto dumpEntityCount = entityList->GetEntityCount();

                for (size_t i = 0; i < dumpEntityCount; i++) {
                    auto entity = entityList->GetEntityByIndex(i);
                    auto actor  = reinterpret_cast<SDK::C_Actor *>(entity);

                    if (!entity)
                        continue;

                    const auto sceneObjectType = entity->GetType();
                    if (std::find(_filterList.begin(), _filterList.end(), sceneObjectType) == _filterList.end())
                        continue;

                    auto sceneObject = entity->GetSceneObject();
                    if (!sceneObject)
                        continue;

                    const char *sceneName = sceneObject->GetName();
                    if (!sceneName)
                        continue;

                    if (strlen(_entityFilter) > 0 && strstr(sceneName, _entityFilter) == nullptr)
                        continue;

                    if (_entityRange > 0.0f && actor->GetPos().dist(localPlayer->GetPos()) >= _entityRange)
                        continue;

                    uniqueNames.insert(sceneName);
                }

                logger->info("=== Entity Names Dump ({} unique entries) ===", uniqueNames.size());
                logger->info("// C++ vector format:");
                std::string vectorFormat = "std::vector<std::string> entityNames = {";
                bool first = true;
                for (const auto &name : uniqueNames) {
                    if (!first) vectorFormat += ", ";
                    vectorFormat += "\"" + name + "\"";
                    first = false;
                }
                vectorFormat += "};";
                logger->info("{}", vectorFormat);

                logger->info("// Line-by-line format:");
                for (const auto &name : uniqueNames) {
                    logger->info("{}", name);
                }
                logger->info("=== End of Dump ===");
            }

            if (ImGui::BeginListBox("Entity types")) {
                size_t i = 0;
                for (auto &[entityType, entityName] : _allTypes) {
                    if (ImGui::Checkbox(entityName.c_str(), &_checkedTypes[i])) {
                        if (_checkedTypes[i])
                            _filterList.push_back(entityType);
                        else
                            _filterList.erase(std::find(_filterList.begin(), _filterList.end(), entityType));
                    }

                    i++;
                }

                ImGui::EndListBox();
            }

            ImGui::NewLine();

            static const char *streamFilterNames[]  = {"None", "Streamed", "Owned"};
            static const char *selectedStreamFilter = streamFilterNames[0];

            if (ImGui::BeginCombo("Streamable filter", selectedStreamFilter)) {
                for (int n = 0; n < IM_ARRAYSIZE(streamFilterNames); n++) {
                    bool is_selected = (selectedStreamFilter == streamFilterNames[n]);
                    if (ImGui::Selectable(streamFilterNames[n], is_selected)) {
                        selectedStreamFilter = streamFilterNames[n];
                        _streamFilter        = static_cast<StreamFilter>(n);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::NewLine();

            ImGui::DragFloat("Range from player filter", &_entityRange, 2.0f, 0.0f, 10000.0f, "%.3f", 1.0f);
            ImGui::InputText("##entityfilter", _entityFilter, 100);
            ImGui::SameLine();
            ImGui::Text("Name filter");

            const auto entityCount = entityList->GetEntityCount();
            if (ImGui::BeginListBox("Entity list")) {
                // bool first = true;
                for (size_t i = 0; i < entityCount; i++) {
                    auto entity = entityList->GetEntityByIndex(i);
                    auto actor  = reinterpret_cast<SDK::C_Actor *>(entity);

                    if (!entity)
                        continue;

                    const auto sceneObjectType = entity->GetType();
                    if (std::find(_filterList.begin(), _filterList.end(), sceneObjectType) == _filterList.end())
                        continue;

                    const auto _filterIter = _allTypes.find(sceneObjectType);

                    auto sceneObject = entity->GetSceneObject();

                    if (!sceneObject)
                        continue;

                    const char *sceneName = sceneObject->GetName();
                    if (!sceneName)
                        continue;

                    if (strlen(_entityFilter) > 0 && strstr(sceneName, _entityFilter) == nullptr)
                        continue;

                    if (_entityRange > 0.0f && actor->GetPos().dist(localPlayer->GetPos()) >= _entityRange)
                        continue;

                    if (_streamFilter != StreamFilter::NONE) {
                        flecs::entity e {};

                        // TODO: better way to detect streamables

                        if (sceneObjectType == SDK::E_EntityType::E_ENTITY_CAR) {
                            const auto veh = reinterpret_cast<SDK::C_Car *>(entity);

                            e = Core::Modules::Vehicle::GetCarEntity(veh);
                        }
                        else if (sceneObjectType == SDK::E_EntityType::E_ENTITY_HUMAN /*|| sceneObjectType == SDK::E_EntityType::E_ENTITY_PLAYER*/) {
                            const auto human = reinterpret_cast<SDK::C_Human2 *>(entity);

                            e = Core::Modules::Human::GetHumanEntity(human);
                        }

                        if (!e.is_valid() || !e.is_alive())
                            continue;

                        if (_streamFilter == StreamFilter::OWNED && !Framework::World::Engine::IsEntityOwner(e, gApplication->GetLocalPlayerOwnerID()))
                            continue;
                    }

                    auto sceneObjectName = fmt::format("{} {} {} ({})", i, sceneName, _filterIter->second, entity->GetType());
                    if (ImGui::Selectable(sceneObjectName.c_str(), _selectedIndex == i)) {
                        _selectedIndex = i;
                    }
                }

                ImGui::EndListBox();
            }

            if (_selectedIndex >= 0 && _selectedIndex <= entityCount) {
                auto inspectedEntity = reinterpret_cast<SDK::C_Actor *>(entityList->GetEntityByIndex(_selectedIndex));
                if (inspectedEntity) {
                    auto sceneObject = reinterpret_cast<SDK::C_Entity *>(inspectedEntity)->GetSceneObject();
                    const char *inspectedName = sceneObject ? sceneObject->GetName() : nullptr;
                    if (inspectedName) {
                        ImGui::Text("Entity name: %s", inspectedName);
                        ImGui::Text("Entity type: %s", _allTypes[inspectedEntity->GetType()].c_str());

                        auto entityPtr = reinterpret_cast<SDK::C_Entity *>(inspectedEntity);
                        auto prefabType = entityPtr->GetPrefabType();
                        ImGui::Text("Prefab type: %s (%d)", GetPrefabTypeName(prefabType), static_cast<int>(prefabType));
                        ImGui::Text("Flags: 0x%08X", entityPtr->GetFlags());
                        ImGui::Text("Name hash: 0x%016llX", entityPtr->GetNameHash());

                        auto entityPos = inspectedEntity->GetPos();
                        auto entityDir = inspectedEntity->GetDir();

                        if (ImGui::DragFloat3("Pos", (float *)&entityPos, 0.1f, -4500.0f, 4500.0f)) {
                            inspectedEntity->SetPos(entityPos);
                        }

                        if (ImGui::DragFloat3("Dir", (float *)&entityDir, 0.01f, -1.0f, 1.0f)) {
                            inspectedEntity->SetDir(entityDir);
                        }

                        if (ImGui::Button("Teleport to entity")) {
                            (reinterpret_cast<SDK::C_Actor *>(localPlayer))->SetPos(entityPos);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Teleport to player")) {
                            inspectedEntity->SetPos(localPlayer->GetPos());
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Deactivate")) {
                            inspectedEntity->Deactivate();
                        }

                        // Component list section
                        ImGui::Separator();
                        if (ImGui::CollapsingHeader("Attached Components", ImGuiTreeNodeFlags_DefaultOpen)) {
                            size_t componentCount = sceneObject->GetComponentCount();
                            ImGui::Text("Component count: %zu", componentCount);

                            if (componentCount > 0 && ImGui::BeginTable("ComponentsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
                                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 50.0f);
                                ImGui::TableSetupColumn("Type Name", ImGuiTableColumnFlags_WidthStretch);
                                ImGui::TableSetupColumn("Type ID", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                                ImGui::TableHeadersRow();

                                for (size_t compIdx = 0; compIdx < componentCount; compIdx++) {
                                    auto component = sceneObject->GetComponentByIndex(compIdx);
                                    if (!component)
                                        continue;

                                    ImGui::TableNextRow();

                                    // Index
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%zu", compIdx);

                                    // Type name
                                    ImGui::TableNextColumn();
                                    const char *typeName = component->GetComponentTypeName();
                                    ImGui::Text("%s", typeName ? typeName : "Unknown");

                                    // Type ID
                                    ImGui::TableNextColumn();
                                    ImGui::Text("0x%08X", component->GetComponentTypeID());
                                }

                                ImGui::EndTable();
                            }
                        }
                    }
                }
            }
        };

        CreateUIWindow("Entity Browser", windowContent, &_open);
    }

    void EntityBrowser::InitialiseEntityTypes() {
        using namespace SDK;
        _allTypes = {{E_EntityType::E_ENTITY_UNKNOWN, "E_ENTITY_UNKNOWN"}, {E_EntityType::E_ENTITY_ENTITY, "E_ENTITY_ENTITY"}, {E_EntityType::E_ENTITY_ENTITY_POS, "E_ENTITY_ENTITY_POS"}, {E_EntityType::E_ENTITY_ENTITY_DUMMY, "E_ENTITY_ENTITY_DUMMY"},
            {E_EntityType::E_ENTITY_ACTOR, "E_ENTITY_ACTOR"}, {E_EntityType::E_ENTITY_TICKED_MODULE, "E_ENTITY_TICKED_MODULE"}, {E_EntityType::E_ENTITY_TICKED_MODULE_MANAGER, "E_ENTITY_TICKED_MODULE_MANAGER"}, {E_EntityType::E_ENTITY_BASE_HUMAN, "E_ENTITY_BASE_HUMAN"},
            {E_EntityType::E_ENTITY_HUMAN, "E_ENTITY_HUMAN"}, {E_EntityType::E_ENTITY_PLAYER, "E_ENTITY_PLAYER"}, {E_EntityType::E_ENTITY_CAR, "E_ENTITY_CAR"}, {E_EntityType::E_ENTITY_CRASH_OBJECT, "E_ENTITY_CRASH_OBJECT"},
            {E_EntityType::E_ENTITY_GLASSBREAKING, "E_ENTITY_GLASSBREAKING"}, {E_EntityType::E_ENTITY_DOOR, "E_ENTITY_DOOR"}, {E_EntityType::E_ENTITY_TREE, "E_ENTITY_TREE"}, {E_EntityType::E_ENTITY_SOUND, "E_ENTITY_SOUND"}, {E_EntityType::E_ENTITY_GRENADE, "E_ENTITY_GRENADE"},
            {E_EntityType::E_ENTITY_MOLOTOV, "E_ENTITY_MOLOTOV"}, {E_EntityType::E_ENTITY_BOAT, "E_ENTITY_BOAT"}, {E_EntityType::E_ENTITY_SLOT_MACHINE, "E_ENTITY_SLOT_MACHINE"}, {E_EntityType::E_ENTITY_ANIMAL, "E_ENTITY_ANIMAL"},
            {E_EntityType::E_ENTITY_STATIC_ENTITY, "E_ENTITY_STATIC_ENTITY"}, {E_EntityType::E_ENTITY_TRANSLOCATED_CAR, "E_ENTITY_TRANSLOCATED_CAR"}, {E_EntityType::E_ENTITY_GARAGE, "E_ENTITY_GARAGE"}, {E_EntityType::E_ENTITY_FRAME_WRAPPER, "E_ENTITY_FRAME_WRAPPER"},
            {E_EntityType::E_ENTITY_ACTOR_DETECTOR, "E_ENTITY_ACTOR_DETECTOR"}, {E_EntityType::E_ENTITY_FAKE_MODEL_TRANSLOCATOR, "E_ENTITY_FAKE_MODEL_TRANSLOCATOR"}, {E_EntityType::E_ENTITY_BLOCKER, "E_ENTITY_BLOCKER"},
            {E_EntityType::E_ENTITY_STATIC_PARTICLE, "E_ENTITY_STATIC_PARTICLE"}, {E_EntityType::E_ENTITY_FIRETARGET, "E_ENTITY_FIRETARGET"}, {E_EntityType::E_ENTITY_CUTSCENE, "E_ENTITY_CUTSCENE"}, {E_EntityType::E_ENTITY_DUMMY, "E_ENTITY_DUMMY"},
            {E_EntityType::E_ENTITY_TELEPHONE, "E_ENTITY_TELEPHONE"}, {E_EntityType::E_ENTITY_SCRIPT, "E_ENTITY_SCRIPT"}, {E_EntityType::E_ENTITY_PATH_OBJECT, "E_ENTITY_PATH_OBJECT"}, {E_EntityType::E_ENTITY_SCRIPTCMDBUFFER, "E_ENTITY_SCRIPTCMDBUFFER"},
            {E_EntityType::E_ENTITY_SCRIPTDELAYBUFFER, "E_ENTITY_SCRIPTDELAYBUFFER"}, {E_EntityType::E_ENTITY_ROADBLOCK, "E_ENTITY_ROADBLOCK"}, {E_EntityType::E_ENTITY_DAMAGE_ZONE, "E_ENTITY_DAMAGE_ZONE"}, {E_EntityType::E_ENTITY_AIRCRAFT, "E_ENTITY_AIRCRAFT"},
            {E_EntityType::E_ENTITY_TV, "E_ENTITY_TV"}, {E_EntityType::E_ENTITY_PINUP, "E_ENTITY_PINUP"}, {E_EntityType::E_ENTITY_SPIKE_STRIP, "E_ENTITY_SPIKE_STRIP"}, {E_EntityType::E_ENTITY_SCRIPT_SHOPMENU, "E_ENTITY_SCRIPT_SHOPMENU"},
            {E_EntityType::E_ENTITY_PHYSICS_SCENE, "E_ENTITY_PHYSICS_SCENE"}, {E_EntityType::E_ENTITY_CLEAN_ENTITY, "E_ENTITY_CLEAN_ENTITY"}, {E_EntityType::E_ENTITY_ITEM_WEAPON, "E_ENTITY_ITEM_WEAPON"}, {E_EntityType::E_ENTITY_ITEM_SCRIPT_AB, "E_ENTITY_ITEM_SCRIPT_AB"},
            {E_EntityType::E_ENTITY_NAVDUMMY, "E_ENTITY_NAVDUMMY"}, {E_EntityType::E_ENTITY_ITEM_INVENTORY, "E_ENTITY_ITEM_INVENTORY"}, {E_EntityType::E_ENTITY_ITEM_UPGRADE, "E_ENTITY_ITEM_UPGRADE"}, {E_EntityType::E_ENTITY_TRAP, "E_ENTITY_TRAP"},
            {E_EntityType::E_ENTITY_NAV_BOX, "E_ENTITY_NAV_BOX"}, {E_EntityType::E_ENTITY_FAKE_ENEMY, "E_ENTITY_FAKE_ENEMY"}, {E_EntityType::E_ENTITY_PIER, "E_ENTITY_PIER"}, {E_EntityType::E_ENTITY_TAIL, "E_ENTITY_TAIL"},
            {E_EntityType::E_ENTITY_BATTLE_DESTINATION, "E_ENTITY_BATTLE_DESTINATION"}, {E_EntityType::E_ENTITY_MOUNTED_WEAPON, "E_ENTITY_MOUNTED_WEAPON"}, {E_EntityType::E_ENTITY_TRAIL, "E_ENTITY_TRAIL"}, {E_EntityType::E_ENTITY_TRAFFIC_SEMAPHORE, "E_ENTITY_TRAFFIC_SEMAPHORE"},
            {E_EntityType::E_ENTITY_HEAD_QUARTERS, "E_ENTITY_HEAD_QUARTERS"}, {E_EntityType::E_ENTITY_CRIME_BUSINESS, "E_ENTITY_CRIME_BUSINESS"}, {E_EntityType::E_ENTITY_BED, "E_ENTITY_BED"}, {E_EntityType::E_ENTITY_PROJECTILE_EXPLOSIVE, "E_ENTITY_PROJECTILE_EXPLOSIVE"},
            {E_EntityType::E_ENTITY_SATCHEL_CHARGE, "E_ENTITY_SATCHEL_CHARGE"}, {E_EntityType::E_ENTITY_LAST_ID, "E_ENTITY_LAST_ID"}};
    }
}; // namespace MafiaMP::Core::UI::Devs
