#include "sdk.h"

#include <logging/logger.h>

namespace MafiaMP::Core::UI::Web {
    JSValueRef OnCallEvent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception) {
        Framework::Logging::GetLogger("Web")->debug("OnCallEvent");

        // Make sure there is only two arguments
        if (argumentCount != 2) {
            // TODO: throw an exception
            Framework::Logging::GetLogger("Web")->error("OnCallEvent: Invalid argument count");
            return JSValueMakeBoolean(ctx, false);
        }

        // Make sure the first argument is a string
        if (!JSValueIsString(ctx, arguments[0])) {
            // TODO: throw an exception
            Framework::Logging::GetLogger("Web")->error("OnCallEvent: first argument must be a string");
            return JSValueMakeBoolean(ctx, false);
        }

        // Make sure the second argument is a string or null (because it is a JSON payload)
        if (!JSValueIsString(ctx, arguments[1]) && !JSValueIsNull(ctx, arguments[1]) && !JSValueIsUndefined(ctx, arguments[1])) {
            // TODO: throw an exception
            Framework::Logging::GetLogger("Web")->error("OnCallEvent: second argument must be a string or null");
            return JSValueMakeBoolean(ctx, false);
        }

        // Grab the event name
        auto eventName = JSValueToStringCopy(ctx, arguments[0], nullptr);
        size_t len = JSStringGetMaximumUTF8CStringSize(eventName) + 1;
        char *eventNameStr = new char[len];
        memset(eventNameStr, 0, len);
        JSStringGetUTF8CString(eventName, eventNameStr, len);

        // Grab the event payload
        auto eventPayload = JSValueToStringCopy(ctx, arguments[1], nullptr);
        size_t len2         = JSStringGetMaximumUTF8CStringSize(eventName) + 1;
        char *eventPayloadStr = new char[len2];
        memset(eventPayloadStr, 0, len2);
        JSStringGetUTF8CString(eventPayload, eventPayloadStr, len2);

        // Grab the SDK instance
        auto sdk = GetPrivateData(thisObject);

        // Process all the event callbacks
        sdk->BroadcastEvent(eventNameStr, eventPayloadStr);

        // Cleanup
        delete[] eventNameStr;
        delete[] eventPayloadStr;

        return JSValueMakeBoolean(ctx, true);
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
        SetPrivateData(_globalObject, this);

        // Bind the callEvent method
        JSStringRef name = JSStringCreateWithUTF8CString("callEvent");
        JSObjectRef func = JSObjectMakeFunctionWithCallback(_context, name, OnCallEvent);
        JSObjectSetProperty(_context, _globalObject, name, func, 0, 0);
        JSStringRelease(name);
    }
}
