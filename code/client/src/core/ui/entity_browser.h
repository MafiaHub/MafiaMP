#pragma once

#include "utils/safe_win32.h"

#include <fu2/function2.hpp>

#include <string>
#include <unordered_map>
#include <vector>

#include "sdk/entities/c_entity.h"

namespace MafiaMP::Core::UI {
    class EntityBrowser final {
      public:
        EntityBrowser();

        void Update();

      private:
        int _selectedIndex = 0;
        std::unordered_map<SDK::E_EntityType, std::string> _allTypes {};

        bool _checkedTypes[66] {};
        std::vector<SDK::E_EntityType> _filterList {};

        float _entityRange      = 200.0f;
        char _entityFilter[100] = "";

        void InitialiseEntityTypes();

        enum class StreamFilter { NONE, STREAMED, OWNED };
        StreamFilter _streamFilter = StreamFilter::NONE;
    };
} // namespace MafiaMP::Core::UI
