#pragma once

#include <cstdint>

namespace MafiaMP::Game::Helpers { 
    namespace UI{
        void DisplayBannerMessage(char const *, char const *);
        void DisplayGenericMessage(char const *, char const *);
        void DisplayTitleCard(char const *, char const *);
        void DisplayMissionExit(char const *, char const *, int);
        void DisplayNote(char const *, char const *);

        void HideTitleCard();
        void HideNotification();

        void ToggleLoadSpinner(bool);

        void ShowNotification(char const *, char const *, int);

        void StartCountdown(int);
    };
} // namespace MafiaMP::Game::Helpers
