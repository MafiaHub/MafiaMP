#include "web.h"

#include <logging/logger.h>
#include <utils/path.h>
#include <utils/string_utils.h>

#include "../../game/module.h"

#include <string>
#include <vector>

#include <cppfs/FileHandle.h>
#include <cppfs/fs.h>

namespace MafiaMP::Core::UI {
    bool Web::Init() {
        // Make sure we have all the required files
        if (!CheckRequiredFiles()) {
            return false;
        }

        // CEF App
        const CefMainArgs args(GetModuleHandleW(nullptr));
        _app = new CEF::Application(*this);

        // CEF Process
        const int32_t exitCode = CefExecuteProcess(args, _app, nullptr);
        if (exitCode >= 0) {
            MessageBoxW(nullptr, L"libcef: CefExecuteProcess failed!", L"Mafia Multiplayer", MB_ICONERROR);
            ExitProcess(0);
        }

        // CEF Settings
        CefSettings settings;
        settings.no_sandbox                   = true;
        settings.multi_threaded_message_loop  = true;
        settings.windowless_rendering_enabled = true;

        // Logging (Debug Build)
        settings.log_severity          = cef_log_severity_t::LOGSEVERITY_DEBUG;
        settings.remote_debugging_port = 8384;

        // Cef Paths
        const auto currentPath = Framework::Utils::GetAbsolutePathW(L"");
        CefString(&settings.log_file).FromWString(Framework::Utils::GetAbsolutePathW(L"logs\\CEF.log"));
        CefString(&settings.root_cache_path).FromWString(Framework::Utils::GetAbsolutePathW(L"cache"));
        CefString(&settings.cache_path).FromWString(Framework::Utils::GetAbsolutePathW(L"cache\\cef"));
        CefString(&settings.framework_dir_path).FromWString(currentPath);
        CefString(&settings.resources_dir_path).FromWString(currentPath);
        CefString(&settings.locales_dir_path).FromWString(Framework::Utils::GetAbsolutePathW(L"locales"));
        CefString(&settings.user_data_path).FromWString(Framework::Utils::GetAbsolutePathW(L"cef\\userdata"));
        CefString(&settings.browser_subprocess_path).FromWString(Framework::Utils::GetAbsolutePathW(L"MafiaMPClientWorker.exe"));

        // Initialize CEF
        if (!CefInitialize(args, settings, _app, nullptr)) {
            MessageBoxW(nullptr, L"libcef: CefInitialize failed!", L"Mafia Multiplayer", MB_ICONERROR);
            return false;
        }

        // Mount our custom scheme handler
        _schemeHandler = new CEF::SchemeHandlerFactory();
        CefRegisterSchemeHandlerFactory("mafiamp", "", _schemeHandler);
        return true;
    }

    bool Web::Shutdown() {
        // Clear our frames
        std::lock_guard<std::mutex> lock(_frameMutex);
        _frames.clear();

        // Shutdown CEF
        CefShutdown();
        return true;
    }

    bool Web::CheckRequiredFiles() {
        std::vector<std::string> filenames = {"resources.pak", "chrome_100_percent.pak", "chrome_200_percent.pak", "icudtl.dat", "locales"};
        for (const auto &filename : filenames) {
            cppfs::FileHandle fh = cppfs::fs::open(Framework::Utils::GetAbsolutePathA(filename));
            if (!fh.exists()) {
                Framework::Logging::GetLogger("CEF")->critical("Required file or directory {} does not exists. Cannot init CEF", filename.c_str());
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<CEF::Frame> Web::AddFrame(const CEF::FrameInfo &info) {
        // Make sure our frame is valid
        if (info.sizeX == 0 || info.sizeY == 0 || !info.url.length()) {
            Framework::Logging::GetLogger("CEF")->error("Failed to create frame due to invalid size or url ({} / {} {})", info.sizeX, info.sizeY, info.url);
            return nullptr;
        }

        // Create a new frame instance and initialize it
        auto frame = std::make_shared<CEF::Frame>(info);
        if (!frame->Initialize()) {
            return nullptr;
        }

        // Add our frame to local manager
        std::lock_guard<std::mutex> lock(_frameMutex);
        _frames.push_back(frame);
        return frame;
    }

    bool Web::RemoveFrame(std::shared_ptr<CEF::Frame> frame) {
        // Find the frame by its pointer
        std::lock_guard<std::mutex> lock(_frameMutex);
        auto it = find(_frames.begin(), _frames.end(), frame);

        // If found, erase
        if (it != _frames.end()) {
            _frames.erase(it);
            return true;
        }

        return false;
    }

    void Web::Render() {
        std::lock_guard<std::mutex> lock(_frameMutex);
        for (auto &frame : _frames) {
            frame->Render();
        }
    }

    void Web::Update() {

    }
} // namespace MafiaMP::Core::UI
