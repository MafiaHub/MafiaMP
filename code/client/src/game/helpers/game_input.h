#pragma once

#include <utils/safe_win32.h>

#include <input/input.h>

namespace MafiaMP::Game {
    class GameInput: public Framework::Input::IInput {
      public:
        GameInput() {}
        ~GameInput() = default;

        void Update() override;

        void GetMousePosition(int &x, int &y) override {};
        void SetMousePosition(int x, int y) override {};
        void SetMouseVisible(bool visible) override {};
        bool IsMouseVisible() override {
            return false;
        };
        void SetMouseLocked(bool locked) override {};
        bool IsMouseLocked() override {
            return false;
        };

        bool IsKeyDown(int key) override {
            return _keysDown[MapKey(key)];
        };
        bool IsKeyUp(int key) override {
            return !_keysDown[MapKey(key)];
        };
        bool IsKeyPressed(int key) override {
            return _keysPressed[MapKey(key)];
        };
        bool IsKeyReleased(int key) override {
            return _keysReleased[MapKey(key)];
        };

        bool IsMouseButtonDown(int button) override {
            return false;
        };
        bool IsMouseButtonUp(int button) override {
            return false;
        };
        bool IsMouseButtonPressed(int button) override {
            return false;
        };
        bool IsMouseButtonReleased(int button) override {
            return false;
        };

        uint32_t MapKey(uint32_t key) override;

        void ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

      private:
        bool _keysDown[256]     = {false};
        bool _keysPressed[256]  = {false};
        bool _keysReleased[256] = {false};
    };
} // namespace MafiaMP::Game
