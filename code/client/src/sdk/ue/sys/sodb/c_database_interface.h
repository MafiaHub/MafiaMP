#pragma once

#include <stdint.h>

namespace SDK {
    namespace ue::sys::sodb {

        class C_Database {
          public:

            // Accessor for table items
            // Todo(Greavesy): add a few operator overloads
            template <typename T>
            class C_AccessorConst {
              public:
                const T *Get() const {
                    return m_Item;
                }

                bool IsValid() const {
                    return m_Item != nullptr;
                }

              private:
                const T *m_Item = nullptr;
            };
        };

        class C_DatabaseInterface {
          public:

            virtual ~C_DatabaseInterface()                               = 0;
            virtual void C_DatabaseEventListener_OnMountPointMounted()   = 0;
            virtual void C_DatabaseEventListener_OnMountPointUnmounted() = 0;
            virtual void *GetClassTypeInfo()                             = 0;
            virtual void *GetTypeInfo()                                  = 0;
            virtual uint32_t GetID()                                     = 0;

          private:

              // 0x20 is an instance of C_Database
              // 0x28 appears to be something from the C_Database
              // 0x30 is an instance of C_XBinMountPoint

        };
    }; // namespace ue::sys::sodb
};     // namespace SDK
