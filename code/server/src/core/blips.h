#pragma once

#include <mafianet/types.h>

#include <glm/glm.hpp>

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace MafiaMP::Core {
    // Map blips, shown by the clients on the map and the radar. The server owns them: scripts
    // create and change them, and every change is pushed to the clients that see the blip.
    class Blips final {
      public:
        struct Blip {
            glm::vec3 position {};
            std::string icon;
            // Entity the blip follows, by NetworkID; 0 for a blip that stays where it is put.
            uint64_t attachedTo = 0;
            // Human whose player alone sees the blip, by NetworkID; 0 for everyone.
            uint64_t visibleTo = 0;
            glm::vec3 sentPosition {};
        };

        uint32_t Create(const glm::vec3 &position, const std::string &icon, uint64_t visibleTo);
        void Destroy(uint32_t id);

        const Blip *Get(uint32_t id) const;
        void SetPosition(uint32_t id, const glm::vec3 &position);
        void SetIcon(uint32_t id, const std::string &icon);
        void AttachTo(uint32_t id, uint64_t entityId);

        // GPS route of a player (its human, by NetworkID) to a blip it sees; 0 clears it.
        void SetRoute(uint64_t humanId, uint32_t id);
        uint32_t GetRoute(uint64_t humanId) const;

        // Moves the blips that follow an entity, and drops those whose player left.
        void Update();

        // Sends a joining player every blip it sees.
        void SendAll(MafiaNet::RakNetGUID guid, uint64_t humanId) const;

        void OnHumanRemoved(uint64_t humanId);

      private:
        void Send(uint32_t id, Blip &blip) const;
        void SendTo(uint32_t id, const Blip &blip, MafiaNet::RakNetGUID guid, bool broadcast) const;
        void SendRemove(uint32_t id, const Blip &blip) const;
        void SendRoute(MafiaNet::RakNetGUID guid, uint32_t id) const;
        bool Target(const Blip &blip, MafiaNet::RakNetGUID &guid, bool &broadcast) const;

        std::unordered_map<uint32_t, Blip> _blips;
        std::unordered_map<uint64_t, uint32_t> _routes;
        uint32_t _nextId = 1;
        std::chrono::steady_clock::time_point _lastUpdate {};
    };
} // namespace MafiaMP::Core
