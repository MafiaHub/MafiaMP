#pragma once

#include <utils/safe_win32.h>

#include <input/input.h>

namespace MafiaMP::Game {
    class GameInput: public Framework::Input::IInput {
      public:
        GameInput() {}
        ~GameInput() = default;

        void Update() override;

        void GetMousePosition(int &x, int &y) override {
            x = _mousePosition[0];
            y = _mousePosition[1];
        };
        void SetMousePosition(int x, int y) override {};
        void SetMouseVisible(bool visible) override {
            SetInputLocked(visible);
        };
        bool IsMouseVisible() override {
            return IsInputLocked();
        };
        void SetMouseLocked(bool locked) override {};
        bool IsMouseLocked() override {
            return false;
        };
        void SetInputLocked(bool locked) override;
        bool IsInputLocked() override;

        bool IsKeyDown(int key) override {
            if (key < 0 || key > 255) {
                return false;
            }
            return _keysDown[MapKey(key)];
        };
        bool IsKeyUp(int key) override {
            if (key < 0 || key > 255) {
                return false;
            }
            return !_keysDown[MapKey(key)];
        };
        bool IsKeyPressed(int key) override {
            if (key < 0 || key > 255) {
                return false;
            }
            return _keysPressed[MapKey(key)];
        };
        bool IsKeyReleased(int key) override {
            if (key < 0 || key > 255) {
                return false;
            }
            return _keysReleased[MapKey(key)];
        };

        bool IsMouseButtonDown(int button) override {
            if (button < 0 || button > 4) {
                return false;
            }
            return _buttonsDown[button];
        };
        bool IsMouseButtonUp(int button) override {
            if (button < 0 || button > 4) {
                return false;
            }
            return !_buttonsDown[button];
        };
        bool IsMouseButtonPressed(int button) override {
            if (button < 0 || button > 4) {
                return false;
            }
            return _buttonsPressed[button];
        };
        bool IsMouseButtonReleased(int button) override {
            if (button < 0 || button > 4) {
                return false;
            }
            return _buttonsReleased[button];
        };

        uint32_t MapKey(uint32_t key) override;

        void ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

      private:
        bool _keysDown[256]     = {false};
        bool _keysPressed[256]  = {false};
        bool _keysReleased[256] = {false};
        bool _buttonsDown[5]    = {false};
        bool _buttonsPressed[5] = {false};
        bool _buttonsReleased[5] = {false};
        int _mousePosition[2]    = {};
    };
} // namespace MafiaMP::Game
