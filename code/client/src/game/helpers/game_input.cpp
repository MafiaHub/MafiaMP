#include "game_input.h"

#include "core/application.h"

namespace MafiaMP::Game {
    void GameInput::Update() {
        for (int i = 0; i < 256; i++) {
            _keysPressed[i]  = false;
            _keysReleased[i] = false;
        }

        for (int i = 0; i < 5; i++) {
            _buttonsPressed[i]  = false;
            _buttonsReleased[i] = false;
        }
    }

    uint32_t GameInput::MapKey(uint32_t key) {
        return key; // we map WndProc VK keys 1:1 to our input keys
    }

    void GameInput::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            _keysDown[wParam]    = true;
            _keysPressed[wParam] = true;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            _keysDown[wParam]     = false;
            _keysReleased[wParam] = true;
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            _buttonsDown[wParam]    = true;
            _buttonsPressed[wParam] = true;
            break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            _buttonsDown[wParam]     = false;
            _buttonsReleased[wParam] = true;
            break;
        case WM_MOUSEMOVE: 
            _mousePosition[0] = LOWORD(lParam);
            _mousePosition[1] = HIWORD(lParam);
            break;
        default: break;
        }
    }

    bool GameInput::IsInputLocked() {
        return Core::gApplication->AreControlsLocked();
    }

    void GameInput::SetInputLocked(bool locked) {
        Core::gApplication->LockControls(locked);
    }

} // namespace MafiaMP::Game
