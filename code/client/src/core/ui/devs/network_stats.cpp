#include "network_stats.h"

#include <slikenet/peer.h>
#include <slikenet/statistics.h>

#include "core/application.h"

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI::Devs {
    void NetworkStats::OnOpen() {}

    void NetworkStats::OnClose() {}

    void NetworkStats::OnUpdate() {
        auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!localPlayer) {
            return;
        }

        auto windowContent = [&]() {
            const auto net   = gApplication->GetNetworkingEngine()->GetNetworkClient();
            const auto state = net->GetConnectionState();

            if (state != Framework::Networking::CONNECTED) {
                ImGui::Text("You are currently not connected to a server!");
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
        };

        CreateUIWindow("Network Stats", windowContent, &_open, ImGuiWindowFlags_AlwaysAutoResize);
    }
} // namespace MafiaMP::Core::UI::Devs
