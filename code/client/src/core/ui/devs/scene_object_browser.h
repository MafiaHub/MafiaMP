#pragma once

#include "../ui_base.h"

#include <string>
#include <vector>

namespace MafiaMP::Core::UI::Devs {
    class SceneObjectBrowser final: public UIBase {
      private:
        char _filterText[256]    = "";
        char _categoryFilter[64] = "";
        int _selectedIndex       = -1;

        struct SceneObjectEntry {
            uint64_t hashName;
            std::string debugPath;
            std::string category;
        };

        std::vector<SceneObjectEntry> _filteredEntries;
        std::vector<std::string> _categories;

        void RefreshFilteredList();

        virtual void OnOpen() override;
        virtual void OnClose() override;
        virtual void OnUpdate() override;

      public:
        SceneObjectBrowser();
    };
} // namespace MafiaMP::Core::UI::Devs
