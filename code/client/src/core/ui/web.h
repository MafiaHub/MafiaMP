#pragma once
#include <utils/safe_win32.h>

#include <mutex>
#include <string>
#include <unordered_map>

#include "cef/app.h"
#include "cef/frame.h"
#include "cef/scheme_handler.h"

namespace MafiaMP::Core::UI {
    class Web {
      private:
        CefRefPtr<CEF::Application> _app;
        CefRefPtr<CEF::SchemeHandlerFactory> _schemeHandler;

        std::list<std::shared_ptr<CEF::Frame>> _frames;
        std::mutex _frameMutex;

      public:
        bool Init();
        bool Shutdown();

        bool CheckRequiredFiles();

        std::shared_ptr<CEF::Frame> AddFrame(const CEF::FrameInfo &info);
        bool RemoveFrame(std::shared_ptr<CEF::Frame> frame);

        void InvokeEvent(const std::string &, const std::string &);

        void Update();
        void Render();
    };
} // namespace MafiaMP::Core::UI
