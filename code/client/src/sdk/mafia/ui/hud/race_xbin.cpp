#include "race_xbin.h"

#include "sdk/mafia/database/c_ui_database.h"
#include "sdk/mafia/ui/c_game_gui_2_module.h"

namespace RaceXBinUtils
{
    // Handy little utility function to fetch the HUDTable, for the Racing XBin.
    SDK::mafia::database::C_UIDatabase::C_HUDTable *GetHUDTable() {

        // Fetch database
        SDK::mafia::ui::C_GameGUI2Module *GameGuiModule = SDK::mafia::ui::GetGameGui2Module();
        SDK::ue::C_WeakPtr<SDK::ue::sys::sodb::C_DatabaseInterface> result = GameGuiModule->GetDatabase();

        // need to cast to C_UIDatabase
        // TODO: Feels like this should be dynamic_cast, rather than reinterpret_cast
        if (SDK::mafia::database::C_UIDatabase *database = reinterpret_cast<SDK::mafia::database::C_UIDatabase *>(result.Get())) {
            return database->GetHUDTable();
        }

        return nullptr;
    }
}

namespace SDK {
    namespace mafia::ui::hud {

        void RaceXBin::SetVisible(const bool visibility) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_bRacingVisible = visibility;
            }
        }

        void RaceXBin::SetTargetTime(const float targetTime) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_fTargetTime = targetTime;
            }
        }

        void RaceXBin::SetPosition(const uint16_t currentPosition) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uCurPosition = currentPosition;
            }
        }

        void RaceXBin::SetPositionTotal(const uint16_t totalPositions) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uTotalPositions = totalPositions;
            }
        }

        void RaceXBin::SetLaps(const uint16_t currentLap) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uCurLap = currentLap;
            }
        }

        void RaceXBin::SetLapsTotal(const uint16_t totalLaps) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uTotalLaps = totalLaps;
            }
        }

        void RaceXBin::SetCheckpoints(const uint16_t currentCheckpoint) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uCurCheckpoint = currentCheckpoint;
            }
        }

        void RaceXBin::SetCheckpointsTotal(const uint16_t totalCheckpoint) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uTotalCheckpoints = totalCheckpoint;
            }
        }

        void RaceXBin::SetCountdown(const uint8_t countdown) {
            if (SDK::mafia::database::C_UIDatabase::C_HUDTable *hudTable = RaceXBinUtils::GetHUDTable()) {
                hudTable->m_uCountdown = countdown;
            }
        }
    } // namespace mafia::ui::hud
} // namespace SDK
