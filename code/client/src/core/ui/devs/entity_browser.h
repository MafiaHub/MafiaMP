#pragma once

#include "../ui_base.h"

#include <string>
#include <unordered_map>
#include <vector>

#include "sdk/entities/c_entity.h"

namespace MafiaMP::Core::UI::Devs {
    class EntityBrowser final: public UIBase {
      private:
        int _selectedIndex = 0;

        std::unordered_map<SDK::E_EntityType, std::string> _allTypes {};

        bool _checkedTypes[66] {};

        std::vector<SDK::E_EntityType> _filterList {};

        float _entityRange = 200.0f;

        char _entityFilter[100] = "";

        void InitialiseEntityTypes();

        enum class StreamFilter {
            NONE,
            STREAMED,
            OWNED
        };

        StreamFilter _streamFilter = StreamFilter::NONE;

        virtual void OnOpen() override;

        virtual void OnClose() override;

        virtual void OnUpdate() override;

      public:
        EntityBrowser();
    };
} // namespace MafiaMP::Core::UI::Devs
