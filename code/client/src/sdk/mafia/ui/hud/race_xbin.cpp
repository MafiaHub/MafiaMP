#include "race_xbin.h"

#include "sdk/mafia/database/c_ui_database.h"
#include "sdk/mafia/ui/c_game_gui_2_module.h"

namespace RaceXBinUtils
{
    // Handy little utility function to fetch the HUDTable, for the Racing XBin.
    SDK::mafia::database::C_UIDatabase::C_HUDTable *GetHUDTable() {
        using namespace SDK::mafia::ui;
        using namespace SDK::mafia::database;
        using namespace SDK::ue::sys::sodb;

        // Fetch database
        C_GameGUI2Module *GameGuiModule = GetGameGui2Module();
        SDK::ue::C_WeakPtr<C_DatabaseInterface> result = GameGuiModule->GetDatabase();

        // need to cast to C_UIDatabase
        // TODO: Feels like this should be dynamic_cast, rather than reinterpret_cast
        if (C_UIDatabase *database = reinterpret_cast<C_UIDatabase *>(result.Get())) {
            return database->GetHUDTable();
        }

        return nullptr;
    }
}

namespace SDK {
    namespace mafia::ui::hud {

        // only has an effect in cpp
        using namespace SDK::mafia::database;

        void RaceXBin::SetVisible(const bool bVisiblity) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_RacingVisible = bVisiblity;
            }
        }

        void RaceXBin::SetTargetTime(const float InTargetTime) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_TargetTime = InTargetTime;
            }
        }

        void RaceXBin::SetPosition(const uint16_t InPosition) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_CurPosition = InPosition;
            }
        }

        void RaceXBin::SetPositionTotal(const uint16_t InTotalPosition) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_TotalPositions = InTotalPosition;
            }
        }

        void RaceXBin::SetLaps(const uint16_t InLaps) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_CurLap = InLaps;
            }
        }

        void RaceXBin::SetLapsTotal(const uint16_t InTotalLaps) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_TotalLaps = InTotalLaps;
            }
        }

        void RaceXBin::SetCheckpoints(const uint16_t InCheckpoint) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_CurCheckpoint = InCheckpoint;
            }
        }

        void RaceXBin::SetCheckpointsTotal(const uint16_t InTotalCheckpoints) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_TotalCheckpoints = InTotalCheckpoints;
            }
        }

        void RaceXBin::SetCountdown(const uint8_t InCountdown) {
            if (C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_Countdown = InCountdown;
            }
        }
    } // namespace mafia::ui::hud
} // namespace SDK
