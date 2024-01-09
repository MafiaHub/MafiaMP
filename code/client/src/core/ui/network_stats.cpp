#include "network_stats.h"
#include "../application.h"

#include <slikenet/peer.h>
#include <slikenet/statistics.h>

#include <external/imgui/wrapper.h>
#include <imgui.h>

#include "game/helpers/controls.h"

namespace MafiaMP::Core::UI {

    NetworkStats::NetworkStats() {}

    void NetworkStats::Update() {
        auto localPlayer = Game::Helpers::Controls::GetLocalPlayer();
        if (!localPlayer)
            return;

        static bool alwaysTrue = true;
        ImGui::Begin("Network Stats", &alwaysTrue, ImGuiWindowFlags_AlwaysAutoResize);
        {
            const auto net   = gApplication->GetNetworkingEngine()->GetNetworkClient();
            const auto state = net->GetConnectionState();

            if (state != Framework::Networking::CONNECTED) {
                ImGui::Text("You are currently not connected to a server!");
                ImGui::End();
                return;
            }

            const auto peer = net->GetPeer();
            const auto stats   = peer->GetStatistics(peer->GetSystemAddressFromIndex(0), nullptr);
            char buffer[8192] = {0};
            SLNet::StatisticsToString(stats, buffer, 2);
            ImGui::Text("%s", buffer);
        }
        ImGui::End();
    }
} // namespace MafiaMP::Core::UI
