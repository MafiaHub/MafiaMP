#pragma once

#include <Ultralight/Ultralight.h>
#include <JavaScriptCore/JavaScript.h>

namespace MafiaMP::Core::UI::Web {
    class SDK {
      private:
        ultralight::View *_view;
        JSContextRef _context;
        JSObjectRef _globalObject;

      public:
        bool Init(ultralight::View *);
        bool Shutdown();

      private:
        void InitEventHandlers();
    };
}
