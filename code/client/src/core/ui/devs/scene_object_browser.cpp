#include "scene_object_browser.h"

#include "core/scene_object_registry.h"

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <logging/logger.h>

#include <algorithm>
#include <set>

namespace MafiaMP::Core::UI::Devs {
    SceneObjectBrowser::SceneObjectBrowser(): UIBase() {}

    void SceneObjectBrowser::OnOpen() {
        RefreshFilteredList();
    }

    void SceneObjectBrowser::OnClose() {}

    void SceneObjectBrowser::RefreshFilteredList() {
        _filteredEntries.clear();
        std::set<std::string> categorySet;

        std::lock_guard<std::mutex> lock(gSceneObjectRegistry.GetMutex());
        const auto &registryMap = gSceneObjectRegistry.GetRegistry();

        std::string filterLower(_filterText);
        std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);

        std::string categoryFilterStr(_categoryFilter);

        for (const auto &[hashName, info] : registryMap) {
            categorySet.insert(info.category);

            // Category filter
            if (!categoryFilterStr.empty() && info.category != categoryFilterStr) {
                continue;
            }

            // Text filter
            if (!filterLower.empty()) {
                std::string pathLower = info.debugPath;
                std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);

                if (pathLower.find(filterLower) == std::string::npos) {
                    continue;
                }
            }

            _filteredEntries.push_back({hashName, info.debugPath, info.category});
        }

        _categories.assign(categorySet.begin(), categorySet.end());

        std::sort(_filteredEntries.begin(), _filteredEntries.end(), [](const SceneObjectEntry &a, const SceneObjectEntry &b) {
            if (a.category != b.category)
                return a.category < b.category;
            return a.debugPath < b.debugPath;
        });
    }

    void SceneObjectBrowser::OnUpdate() {
        auto windowContent = [&]() {
            size_t totalCount = 0;
            {
                std::lock_guard<std::mutex> lock(gSceneObjectRegistry.GetMutex());
                totalCount = gSceneObjectRegistry.GetRegistry().size();
            }

            ImGui::Text("Total scene objects: %zu", totalCount);
            ImGui::Text("Filtered: %zu", _filteredEntries.size());

            ImGui::Separator();

            if (ImGui::InputText("Filter", _filterText, sizeof(_filterText))) {
                RefreshFilteredList();
            }

            ImGui::SameLine();

            ImGui::SetNextItemWidth(150.0f);
            if (ImGui::BeginCombo("Category", _categoryFilter[0] ? _categoryFilter : "All")) {
                if (ImGui::Selectable("All", _categoryFilter[0] == '\0')) {
                    _categoryFilter[0] = '\0';
                    RefreshFilteredList();
                }
                for (const auto &cat : _categories) {
                    bool isSelected = (cat == _categoryFilter);
                    if (ImGui::Selectable(cat.c_str(), isSelected)) {
                        strncpy(_categoryFilter, cat.c_str(), sizeof(_categoryFilter) - 1);
                        _categoryFilter[sizeof(_categoryFilter) - 1] = '\0';
                        RefreshFilteredList();
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();

            if (ImGui::Button("Refresh")) {
                RefreshFilteredList();
            }

            ImGui::SameLine();

            if (ImGui::Button("Copy All")) {
                std::string allPaths;
                for (const auto &entry : _filteredEntries) {
                    allPaths += fmt::format("[{}] 0x{:016X}: {}\n", entry.category, entry.hashName, entry.debugPath);
                }
                ImGui::SetClipboardText(allPaths.c_str());
            }

            ImGui::SameLine();

            if (ImGui::Button("Dump to Console")) {
                auto logger = Framework::Logging::GetLogger("SceneObjectBrowser");
                logger->info("=== Scene Objects Dump ({} entries) ===", _filteredEntries.size());
                for (const auto &entry : _filteredEntries) {
                    logger->info("[{}] 0x{:016X}: {}", entry.category, entry.hashName, entry.debugPath);
                }
                logger->info("=== End of Dump ===");
            }

            ImGui::Separator();

            if (ImGui::BeginTable("SceneObjectsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Sortable, ImVec2(0, ImGui::GetContentRegionAvail().y - 80))) {
                ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Hash", ImGuiTableColumnFlags_WidthFixed, 160.0f);
                ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                ImGuiListClipper clipper;
                clipper.Begin(static_cast<int>(_filteredEntries.size()));

                while (clipper.Step()) {
                    for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                        const auto &entry = _filteredEntries[row];

                        ImGui::TableNextRow();

                        ImGui::TableNextColumn();
                        bool isSelected = (_selectedIndex == row);
                        if (ImGui::Selectable(entry.category.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                            _selectedIndex = row;
                        }

                        ImGui::TableNextColumn();
                        ImGui::Text("0x%016llX", entry.hashName);

                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(entry.debugPath.c_str());
                    }
                }

                ImGui::EndTable();
            }

            if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(_filteredEntries.size())) {
                ImGui::Separator();
                const auto &selectedEntry = _filteredEntries[_selectedIndex];
                ImGui::Text("Selected:");
                ImGui::Text("  Category: %s", selectedEntry.category.c_str());
                ImGui::Text("  Hash: 0x%016llX", selectedEntry.hashName);
                ImGui::Text("  Path: %s", selectedEntry.debugPath.c_str());

                if (ImGui::Button("Copy Hash")) {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "0x%016llX", selectedEntry.hashName);
                    ImGui::SetClipboardText(buf);
                }

                ImGui::SameLine();

                if (ImGui::Button("Copy Path")) {
                    ImGui::SetClipboardText(selectedEntry.debugPath.c_str());
                }
            }
        };

        CreateUIWindow("Scene Object Browser", windowContent, &_open);
    }
} // namespace MafiaMP::Core::UI::Devs
