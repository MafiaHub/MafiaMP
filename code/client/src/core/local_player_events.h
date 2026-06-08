#pragma once

#include <glm/glm.hpp>

namespace MafiaMP::Core::Modules {
    class Vehicle;
} // namespace MafiaMP::Core::Modules

namespace MafiaMP::Core {
    // Central client-side dispatcher for local-player game events raised by the game hooks. The hooks
    // stay thin (they just detect the game event and forward here); this is the single place that
    // owns the matching net broadcast to the server, and the natural seam to also raise client-side
    // scripting events.
    class LocalPlayerEvents {
      public:
        void Died();
        void Shot(const glm::vec3 &aimPos, const glm::vec3 &aimDir, bool unk0, bool unk1);
        void Reloaded(int mode);
        void EnteredVehicle(Modules::Vehicle *vehicle, int seatIndex);
        void LeftVehicle(Modules::Vehicle *vehicle);
    };
} // namespace MafiaMP::Core
