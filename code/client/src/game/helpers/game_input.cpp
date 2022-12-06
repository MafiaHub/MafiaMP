#include "game_input.h"

namespace MafiaMP::Game {
    void GameInput::Update() {}

    uint32_t GameInput::MapKey(uint32_t key) {
        return key; // we map WndProc VK keys 1:1 to our input keys
    }

    void GameInput::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {}
} // namespace MafiaMP::Game
