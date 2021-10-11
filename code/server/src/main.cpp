#include "core/server.h"

int main(int argc, char **argv) {
    Framework::Integrations::Server::InstanceOptions opts;
    opts.bindHost      = "0.0.0.0";
    opts.bindPort      = 27015;
    opts.bindName      = "Mafia MP Local Dev Server";
    opts.maxPlayers    = 10;
    opts.modName       = "MafiaMP";
    opts.modSlug       = "mafia_mp";
    opts.modVersion    = "0.1.0";
    opts.bindPassword  = "";
    opts.enableSignals = true;
    opts.argc = argc;
    opts.argv = argv;

    MafiaMP::Server server;
    if (server.Init(opts) != Framework::Integrations::Server::ServerError::SERVER_NONE) {
        return 1;
    }
    server.Run();
    server.Shutdown();
    return 0;
}