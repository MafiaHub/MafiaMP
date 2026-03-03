#include "core/server.h"

#include "shared/version.h"

int main(int argc, char **argv) {
    Framework::Integrations::Server::InstanceOptions opts;
    opts.bindHost      = "0.0.0.0";
    opts.bindPort      = 27015;
    opts.webBindHost   = "0.0.0.0";
    opts.webBindPort   = 27016;
    opts.maxPlayers    = 10;
    opts.modName       = "MafiaMP";
    opts.modSlug       = "mafiamp_server";
    opts.gameName      = "Mafia: Definitive Edition";
    opts.gameVersion   = "3168979183";
    opts.modVersion    = MafiaMP::Version::rel;
    opts.bindPassword  = "";
    opts.enableSignals = true;
    opts.bindSecretKey = "qskdnkjsqndq12312312qsdqsds";

    opts.services.masterlistUrl = "https://masterlist.mafia.mp";

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
