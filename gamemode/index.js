// Modules
const vehicles = require("./src/modules/vehicles.js");
const players = require("./src/modules/players.js");
const enviroment = require("./src/modules/enviroment.js");
// Data
const data = require("./src/data/data.js");
// Handlers
const commands = require("./src/handlers/commands.js");

const DEFAULT_WEATHER = "temp_teaser_trailer";

sdk.on("gamemodeLoaded", () => {
    // Logs
    console.log("[GAMEMODE] Racing gamemode loaded!");

    // Enviroment
    sdk.Environment.setWeather(DEFAULT_WEATHER);
    vehicles.spawnCars(data.vehicleSpawns);
});

sdk.on("playerConnected", (player) => {
    console.log(`[GAMEMODE] Player ${player.getNickname()} connected!`);
    player.sendChatToAll(
        `[SERVER] ${player.getNickname()} has joined the session!`
    );

    // player.addWeapon(20, 200); // TODO: Not working yet
    player.setPosition(sdk.Vector3(-939.8515, -224.0868, 2.7747));
    player.sendChat(`[SERVER] Welcome ${player.getNickname()}!`);
});

sdk.on("playerDisconnected", (player) => {
    console.log(`[GAMEMODE] Player ${player.getNickname()} disconnected!`);
    player.sendChatToAll(
        `[SERVER] ${player.getNickname()} has left the session!`
    );
});

sdk.on("playerDied", (player) => {
    console.log(`[GAMEMODE] Player ${player.getNickname()} died!`);
    player.sendChatToAll(`[SERVER] Player ${player.getNickname()} died!`);

    // Respawn the player
    player.setHealth(100.0);
    player.setPosition(sdk.Vector3(-939.8515, -224.0868, 2.7747));
});
