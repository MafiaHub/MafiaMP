#pragma once
#include <Ultralight/Ultralight.h>

#include <Ultralight/platform/Clipboard.h>

namespace MafiaMP::Core::UI::Web {
    class SystemClipboard final: public ultralight::Clipboard {
      public:
        void Clear() override;
        ultralight::String ReadPlainText() override;
        void WritePlainText(const ultralight::String &text) override;
    };
} // namespace MafiaMP::Core::UI::Web
