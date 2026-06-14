#pragma once

#include <v8.h>

#include "ui.h"

namespace MafiaMP::Scripting {

    class Builtins final {
      public:
        static void Register(v8::Isolate *isolate, v8::Local<v8::Context> context) {
            UI::Register(isolate, context);
        }
    };

} // namespace MafiaMP::Scripting
