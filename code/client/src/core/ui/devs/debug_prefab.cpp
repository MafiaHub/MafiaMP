#include "debug_prefab.h"

#include <imgui/imgui.h>

#include <sdk/prefab/c_prefab_manager.h>

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

    static const char *s_prefabTypeNames[] = {
        "All",
        "Base",
        "Car",
        "CrashObj",
        "ActorDeform",
        "Wheel",
        "Door",
        "Obsolete_Lift",
        "Boat",
        "Obsolete_Wagon",
        "AIBrain",
        "MountedWeapon",
        "TrafficSemaphore",
        "None"
    };

    static SDK::prefab::E_PREFAB_TYPE s_prefabTypeValues[] = {
        static_cast<SDK::prefab::E_PREFAB_TYPE>(-1), // All
        SDK::prefab::E_PT_Base,
        SDK::prefab::E_PT_Car,
        SDK::prefab::E_PT_CrashObj,
        SDK::prefab::E_PT_ActorDeform,
        SDK::prefab::E_PT_Wheel,
        SDK::prefab::E_PT_Door,
        SDK::prefab::E_PT_Obsolete_Lift,
        SDK::prefab::E_PT_Boat,
        SDK::prefab::E_PT_Obsolete_Wagon,
        SDK::prefab::E_PT_AIBrain,
        SDK::prefab::E_PT_MountedWeapon,
        SDK::prefab::E_PT_TrafficSemaphore,
        SDK::prefab::E_PT_None
    };

    void DebugPrefab::OnOpen() {}

    void DebugPrefab::OnClose() {}

    void DebugPrefab::OnUpdate() {
        auto prefabManager = SDK::prefab::C_PrefabManager::GetInstance();
        if (!prefabManager) {
            return;
        }

        auto windowContent = [&]() {
            const auto &prefabs     = prefabManager->GetPrefabs();
            const size_t totalCount = prefabs.size();

            ImGui::Text("Total prefab entries: %zu", totalCount);
            ImGui::Separator();

            // Type filter combo
            static int selectedTypeIndex = 0;
            if (ImGui::Combo("Type Filter", &selectedTypeIndex, s_prefabTypeNames, IM_ARRAYSIZE(s_prefabTypeNames))) {
                _typeFilter = static_cast<int>(s_prefabTypeValues[selectedTypeIndex]);
            }

            // GUID filter
            ImGui::InputText("GUID Filter (hex)", _guidFilter, sizeof(_guidFilter));

            uint64_t guidFilterValue   = 0;
            bool hasGuidFilter         = false;
            if (strlen(_guidFilter) > 0) {
                char *endPtr         = nullptr;
                guidFilterValue      = strtoull(_guidFilter, &endPtr, 16);
                hasGuidFilter        = (endPtr != _guidFilter);
            }

            ImGui::Separator();

            // Count visible entries
            size_t visibleCount = 0;
            for (const auto &entry : prefabs) {
                if (_typeFilter >= 0 && entry.m_eType != static_cast<SDK::prefab::E_PREFAB_TYPE>(_typeFilter)) {
                    continue;
                }
                if (hasGuidFilter && entry.m_uGUID != guidFilterValue) {
                    continue;
                }
                visibleCount++;
            }

            ImGui::Text("Showing: %zu / %zu", visibleCount, totalCount);

            // Table header
            if (ImGui::BeginTable("PrefabTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0, 400))) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("GUID", ImGuiTableColumnFlags_WidthFixed, 180.0f);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Data Count", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableHeadersRow();

                int index = 0;
                for (const auto &entry : prefabs) {
                    // Apply filters
                    if (_typeFilter >= 0 && entry.m_eType != static_cast<SDK::prefab::E_PREFAB_TYPE>(_typeFilter)) {
                        index++;
                        continue;
                    }
                    if (hasGuidFilter && entry.m_uGUID != guidFilterValue) {
                        index++;
                        continue;
                    }

                    ImGui::TableNextRow();

                    // Index
                    ImGui::TableNextColumn();
                    char label[32];
                    snprintf(label, sizeof(label), "%d", index);
                    bool isSelected = (_selectedIndex == index);
                    if (ImGui::Selectable(label, isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                        _selectedIndex = index;
                    }

                    // GUID
                    ImGui::TableNextColumn();
                    ImGui::Text("0x%016llX", entry.m_uGUID);

                    // Type
                    ImGui::TableNextColumn();
                    ImGui::Text("%s (%d)", GetPrefabTypeName(entry.m_eType), static_cast<int>(entry.m_eType));

                    // Data count
                    ImGui::TableNextColumn();
                    ImGui::Text("%zu", entry.m_aData.size());

                    index++;
                }

                ImGui::EndTable();
            }

            // Selected entry details
            if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(totalCount)) {
                ImGui::Separator();
                ImGui::Text("Selected Entry Details:");

                const auto &entry = prefabs[_selectedIndex];
                ImGui::Text("  GUID: 0x%016llX", entry.m_uGUID);
                ImGui::Text("  Type: %s (%d)", GetPrefabTypeName(entry.m_eType), static_cast<int>(entry.m_eType));
                ImGui::Text("  Owned: %s", entry.m_bOwned ? "Yes" : "No");
                ImGui::Text("  Data Pointers: %zu", entry.m_aData.size());

                if (!entry.m_aData.empty()) {
                    if (ImGui::TreeNode("Data Pointers")) {
                        for (size_t i = 0; i < entry.m_aData.size(); i++) {
                            ImGui::Text("    [%zu] 0x%p", i, entry.m_aData[i]);
                        }
                        ImGui::TreePop();
                    }
                }
            }
        };

        CreateUIWindow("Prefab Manager", windowContent, &_open, ImGuiWindowFlags_None);
    }
} // namespace MafiaMP::Core::UI::Devs
