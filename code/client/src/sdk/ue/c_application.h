#pragma once

namespace SDK {
    namespace ue {
        class I_Application {
          public:
            virtual ~I_Application() = default;
        };

        class C_Application : public I_Application {

        };
    }
}
