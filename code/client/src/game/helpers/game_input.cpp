#include "game_input.h"

#include "core/application.h"

namespace MafiaMP::Game {
    namespace {
        // The Framework's button numbering: left, right, middle, X1, X2. A mouse message's wParam
        // holds MK_* key-state flags, never a button number.
        int MouseButtonIndex(UINT msg, WPARAM wParam) {
            switch (msg) {
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP: return 0;
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP: return 1;
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP: return 2;
            default: return GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 3 : 4;
            }
        }
    } // namespace

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

    uint32_t GameInput::MapKey(uint32_t key) const {
        return key; // we map WndProc VK keys 1:1 to our input keys
    }

    void GameInput::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            // Bit 30 is set on the repeats Windows sends while a key is held: a press is the first
            // keydown only, or a held Enter reopens the chat it has just sent.
            if ((lParam & (1 << 30)) == 0) {
                _keysPressed[wParam] = true;
            }
            _keysDown[wParam] = true;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            _keysDown[wParam]     = false;
            _keysReleased[wParam] = true;
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN: {
            const int button        = MouseButtonIndex(msg, wParam);
            _buttonsDown[button]    = true;
            _buttonsPressed[button] = true;
        } break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP: {
            const int button         = MouseButtonIndex(msg, wParam);
            _buttonsDown[button]     = false;
            _buttonsReleased[button] = true;
        } break;
        case WM_MOUSEMOVE: 
            _mousePosition[0] = LOWORD(lParam);
            _mousePosition[1] = HIWORD(lParam);
            break;
        default: break;
        }
    }

    bool GameInput::IsInputLocked() const {
        return Core::gApplication->AreControlsLocked();
    }

    void GameInput::SetInputLocked(bool locked) {
        Core::gApplication->LockControls(locked);
    }

} // namespace MafiaMP::Game
