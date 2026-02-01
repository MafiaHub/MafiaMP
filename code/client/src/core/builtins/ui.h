#pragma once

#include <v8.h>

namespace MafiaMP::Scripting {

    /**
     * UI builtins for MafiaMP client.
     * Exposes game UI functions (banners, notifications, fader) to JavaScript.
     */
    class UI final {
      public:
        static void Register(v8::Isolate *isolate, v8::Local<v8::Context> context);

      private:
        static void DisplayBannerMessageCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void DisplayGenericMessageCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void DisplayTitleCardCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void HideTitleCardCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void DisplayNoteCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void ToggleSaveLoadSpinnerCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void ShowNotificationCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void HideNotificationCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void DisplayMissionExitCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void StartCountdownCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void FaderFadeInCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void FaderFadeOutCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void FaderResetCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    };

} // namespace MafiaMP::Scripting
