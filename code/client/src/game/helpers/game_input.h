#pragma once

#include <utils/safe_win32.h>

#include <input/window_input.h>

namespace MafiaMP::Game {
    class GameInput final: public Framework::Input::WindowInput {
      public:
        GameInput() {}
        ~GameInput() = default;

        void SetMousePosition(int x, int y) override {};
        void SetMouseVisible(bool visible) override {
            SetInputLocked(visible);
        };
        bool IsMouseVisible() const override {
            return IsInputLocked();
        };
        void SetMouseLocked(bool locked) override {};
        bool IsMouseLocked() const override {
            return false;
        };
        void SetInputLocked(bool locked) override;
        bool IsInputLocked() const override;
    };
} // namespace MafiaMP::Game
