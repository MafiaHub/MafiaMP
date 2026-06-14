#include "ui.h"

#include <v8pp/convert.hpp>

#include <game/helpers/ui.h>
#include <sdk/mafia/ui/c_game_gui_2_module.h>

namespace MafiaMP::Scripting {

    void UI::Register(v8::Isolate *isolate, v8::Local<v8::Context> context) {
        v8::Local<v8::Object> uiObj = v8::Object::New(isolate);

        auto setMethod = [&](const char *name, v8::FunctionCallback callback) {
            v8::Local<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New(isolate, callback);
            uiObj->Set(context, v8pp::to_v8(isolate, name), tmpl->GetFunction(context).ToLocalChecked()).Check();
        };

        setMethod("displayBannerMessage", DisplayBannerMessageCallback);
        setMethod("displayGenericMessage", DisplayGenericMessageCallback);
        setMethod("displayTitleCard", DisplayTitleCardCallback);
        setMethod("hideTitleCard", HideTitleCardCallback);
        setMethod("displayNote", DisplayNoteCallback);
        setMethod("toggleSaveLoadSpinner", ToggleSaveLoadSpinnerCallback);
        setMethod("showNotification", ShowNotificationCallback);
        setMethod("hideNotification", HideNotificationCallback);
        setMethod("displayMissionExit", DisplayMissionExitCallback);
        setMethod("startCountdown", StartCountdownCallback);
        setMethod("faderFadeIn", FaderFadeInCallback);
        setMethod("faderFadeOut", FaderFadeOutCallback);
        setMethod("faderReset", FaderResetCallback);

        context->Global()->Set(context, v8pp::to_v8(isolate, "UI"), uiObj).Check();
    }

    void UI::DisplayBannerMessageCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsString()) {
            return;
        }
        std::string title = v8pp::from_v8<std::string>(isolate, args[0]);
        std::string message = v8pp::from_v8<std::string>(isolate, args[1]);
        MafiaMP::Game::Helpers::UI::DisplayBannerMessage(title.c_str(), message.c_str());
    }

    void UI::DisplayGenericMessageCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsString()) {
            return;
        }
        std::string title = v8pp::from_v8<std::string>(isolate, args[0]);
        std::string message = v8pp::from_v8<std::string>(isolate, args[1]);
        MafiaMP::Game::Helpers::UI::DisplayGenericMessage(title.c_str(), message.c_str());
    }

    void UI::DisplayTitleCardCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsString()) {
            return;
        }
        std::string title = v8pp::from_v8<std::string>(isolate, args[0]);
        std::string message = v8pp::from_v8<std::string>(isolate, args[1]);
        MafiaMP::Game::Helpers::UI::DisplayTitleCard(title.c_str(), message.c_str());
    }

    void UI::HideTitleCardCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        MafiaMP::Game::Helpers::UI::HideTitleCard();
    }

    void UI::DisplayNoteCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2 || !args[0]->IsString() || !args[1]->IsString()) {
            return;
        }
        std::string title = v8pp::from_v8<std::string>(isolate, args[0]);
        std::string message = v8pp::from_v8<std::string>(isolate, args[1]);
        MafiaMP::Game::Helpers::UI::DisplayNote(title.c_str(), message.c_str());
    }

    void UI::ToggleSaveLoadSpinnerCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 1 || !args[0]->IsBoolean()) {
            return;
        }
        bool toggle = v8pp::from_v8<bool>(isolate, args[0]);
        MafiaMP::Game::Helpers::UI::ToggleLoadSpinner(toggle);
    }

    void UI::ShowNotificationCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 3 || !args[0]->IsString() || !args[1]->IsString() || !args[2]->IsNumber()) {
            return;
        }
        std::string title = v8pp::from_v8<std::string>(isolate, args[0]);
        std::string message = v8pp::from_v8<std::string>(isolate, args[1]);
        int color = v8pp::from_v8<int>(isolate, args[2]);
        MafiaMP::Game::Helpers::UI::ShowNotification(title.c_str(), message.c_str(), color);
    }

    void UI::HideNotificationCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        MafiaMP::Game::Helpers::UI::HideNotification();
    }

    void UI::DisplayMissionExitCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 3 || !args[0]->IsString() || !args[1]->IsString() || !args[2]->IsNumber()) {
            return;
        }
        std::string title = v8pp::from_v8<std::string>(isolate, args[0]);
        std::string message = v8pp::from_v8<std::string>(isolate, args[1]);
        int time = v8pp::from_v8<int>(isolate, args[2]);
        MafiaMP::Game::Helpers::UI::DisplayMissionExit(title.c_str(), message.c_str(), time);
    }

    void UI::StartCountdownCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 1 || !args[0]->IsNumber()) {
            return;
        }
        int time = v8pp::from_v8<int>(isolate, args[0]);
        MafiaMP::Game::Helpers::UI::StartCountdown(time);
    }

    void UI::FaderFadeInCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsBoolean()) {
            return;
        }
        float duration = v8pp::from_v8<float>(isolate, args[0]);
        bool autoFadeOut = v8pp::from_v8<bool>(isolate, args[1]);

        const auto fader = SDK::mafia::ui::GetGameGui2Module()->GetFader();
        if (fader) {
            SDK::ue::C_CntPtr<uintptr_t> syncObject;
            fader->FadeIn(syncObject, duration, "CustomFaderFadeIn", autoFadeOut);
        }
    }

    void UI::FaderFadeOutCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        v8::Isolate *isolate = args.GetIsolate();
        if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsBoolean()) {
            return;
        }
        float duration = v8pp::from_v8<float>(isolate, args[0]);
        bool autoFadeIn = v8pp::from_v8<bool>(isolate, args[1]);

        const auto fader = SDK::mafia::ui::GetGameGui2Module()->GetFader();
        if (fader) {
            SDK::ue::C_CntPtr<uintptr_t> syncObject;
            fader->FadeOut(syncObject, duration, "CustomFaderFadeOut", autoFadeIn);
        }
    }

    void UI::FaderResetCallback(const v8::FunctionCallbackInfo<v8::Value> &args) {
        const auto fader = SDK::mafia::ui::GetGameGui2Module()->GetFader();
        if (fader) {
            fader->Reset();
        }
    }

} // namespace MafiaMP::Scripting
