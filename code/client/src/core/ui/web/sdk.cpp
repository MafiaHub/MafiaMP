#include "sdk.h"

#include <logging/logger.h>

namespace MafiaMP::Core::UI::Web {
    JSValueRef OnCallEvent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception) {
        Framework::Logging::GetLogger("Web")->debug("OnCallEvent");

        return JSValueMakeNull(ctx);
    }

    bool SDK::Init(ultralight::View* caller) {
        // Bind the view reference
        _view = caller;

        // Initialize our context
        auto context = _view->LockJSContext();
        _context = (*context);

        // Bind our global object
        _globalObject = JSContextGetGlobalObject(_context);

        // Initialize the builtin SDK
        InitEventHandlers();
        return true;
    }

    bool SDK::Shutdown() {
        return true;
    }

    void SDK::InitEventHandlers() {
        // Bind the callEvent method
        JSStringRef name = JSStringCreateWithUTF8CString("callEvent");
        JSObjectRef func = JSObjectMakeFunctionWithCallback(_context, name, OnCallEvent);
        JSObjectSetProperty(_context, _globalObject, name, func, 0, 0);
        JSStringRelease(name);
    }
}
