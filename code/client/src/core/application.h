#pragma once

#include <integrations/client/instance.h>

namespace MafiaMP::Core {
    class Application: public Framework::Integrations::Client::Instance {};

    extern std::unique_ptr<Application> gApplication;
} // namespace MafiaMP::Core
