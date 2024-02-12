#pragma once

namespace MafiaMP::Core::UI {
    class AudioDebug final {
      public:
        AudioDebug();

        void Update();

        bool IsVisible() const {
            return _visible;
        }

        void SetVisible(bool visible) {
            _visible = visible;
        }

      private:
        bool _visible = false;
    };
} // namespace MafiaMP::Core::UI
