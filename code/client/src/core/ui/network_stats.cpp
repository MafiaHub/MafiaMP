#include "network_stats.h"
#include "../application.h"

#include <slikenet/peer.h>
#include <slikenet/statistics.h>

#include <imgui.h>

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI {
    NetworkStats::NetworkStats() {}

    void NetworkStats::Update() {
        auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!localPlayer)
            return;

        ImGui::Begin("Network Stats", &_visible, ImGuiWindowFlags_AlwaysAutoResize);
        {
            const auto net   = gApplication->GetNetworkingEngine()->GetNetworkClient();
            const auto state = net->GetConnectionState();

            if (state != Framework::Networking::CONNECTED) {
                ImGui::Text("You are currently not connected to a server!");
                ImGui::End();
                return;
            }

            if (_nextStatsUpdate < Framework::Utils::Time::GetTime()) {
                _nextStatsUpdate = Framework::Utils::Time::GetTime() + 500;

                const auto peer  = net->GetPeer();
                const auto stats = peer->GetStatistics(peer->GetSystemAddressFromIndex(0), nullptr);
                ::memset(_stats, 0, sizeof(_stats));
                SLNet::StatisticsToString(stats, _stats, 2);
            }

            ImGui::Text("%s", _stats);
        }
        ImGui::End();
    }
} // namespace MafiaMP::Core::UI
