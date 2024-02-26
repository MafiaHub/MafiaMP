#include "sdk/mafia/database/c_ui_database.h"

#include "../../patterns.h"

namespace SDK {
    namespace mafia::database {
        C_UIDatabase::C_HUDTable* C_UIDatabase::GetHUDTable() {
            return m_HUDTable;
        }
    }
}
