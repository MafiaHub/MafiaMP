#pragma once

#include <cstddef>
#include <cstdint>

namespace SDK {
    namespace mafia::ui::navigation {
        // An icon registered with the navigation: what the map and the radar draw, and what the GPS
        // routes to. Allocated by C_Navigation::CreateEmptyNavEntity (0x220 bytes).
        struct S_Entity {
          public:
            // m_uVisibilityFlags. The script bindings SetIconRadarVisibilityByNavId and
            // SetIconMapVisibilityByNavId toggle one bit each; RegisterIconPos takes the field whole.
            enum VisibilityFlags : uint16_t {
                kRadarVisible = 0x1,
                kMapVisible   = 0x2,
            };

            uint32_t GetId() const {
                return m_uId;
            }

            void SetRadarVisible(bool visible) {
                SetVisibilityFlag(kRadarVisible, visible);
            }

            void SetMapVisible(bool visible) {
                SetVisibilityFlag(kMapVisible, visible);
            }

            // Read by C_Navigation::EnableEntityGPS: the GPS only builds a route to an entity that has
            // it set. The script binding SetIconGPSEnabledByNavId writes it.
            void SetGPSEnabled(bool enabled) {
                m_bGPSEnabled = enabled;
            }

          private:
            void SetVisibilityFlag(VisibilityFlags flag, bool set) {
                m_uVisibilityFlags = static_cast<uint16_t>(set ? (m_uVisibilityFlags | flag) : (m_uVisibilityFlags & ~flag));
            }

            char _pad0[0x50];            // 0x00 - 0x50
            uint32_t m_uId;              // 0x50 - 0x54
            uint16_t m_uVisibilityFlags; // 0x54 - 0x56
            char _pad56[0x84 - 0x56];    // 0x56 - 0x84
            bool m_bGPSEnabled;          // 0x84 - 0x85
            char _pad85[0x220 - 0x85];   // 0x85 - 0x220

            static void AssertLayout() {
                static_assert(sizeof(S_Entity) == 0x220, "S_Entity size mismatch");
                static_assert(offsetof(S_Entity, m_uId) == 0x50, "S_Entity::m_uId offset mismatch");
                static_assert(offsetof(S_Entity, m_uVisibilityFlags) == 0x54, "S_Entity::m_uVisibilityFlags offset mismatch");
                static_assert(offsetof(S_Entity, m_bGPSEnabled) == 0x84, "S_Entity::m_bGPSEnabled offset mismatch");
            }
        };
    } // namespace mafia::ui::navigation
} // namespace SDK
