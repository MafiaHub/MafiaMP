const teleports = require("../modules/teleports.js");
const vehicles = require("../modules/vehicles.js");

console.log("[GAMEMODE|HANDLERS] commands.js loaded!");

sdk.on("chatCommand", (player, message, command, args) => {
    console.log(
        `[GAMEMODE] Player ${player.getNickname()} used: ${command}. (${message})`
    );

    switch (command) {
        // Teleports
        case "spawn":
            player.setPosition(sdk.Vector3(-939.8515, -224.0868, 2.7747));
            player.sendChat(`[SERVER] Teleported to home!`);
            break;

        case "tp":
            teleports.teleport(player, args);
            break;

        case "pos":
            console.log(player.getPosition().toString());
            player.sendChat(
                `[SERVER] Player ${player.getNickname()} position: ${player
                    .getPosition()
                    .toString()}`
            );
            break;

        // Vehicles
        case "plates":
            vehicles.setPlates(player, args[0]);
            break;

        // Stuff
        case "args":
            sdk.Chat.sendToAll(
                `[SERVER] Player ${player.getNickname()} used /args with args: ${args}`
            );
            break;

        default:
            sdk.Chat.sendToPlayer(
                player,
                `[SERVER] Unknown command "${command}"`
            );
            break;
    }
});
