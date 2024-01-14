const data = require("../data/data.js");

console.log("[GAMEMODE|MODULES] teleports.js loaded!");

const teleport = function (player, args) {
    const playerVehicle = player.getVehicle();

    const isStringSpot = /[a-zA-Z]/.test(args);
    const isNumericCoords = args.length >= 3;

    if (isStringSpot || isNumericCoords) {
        const SPOTS = data.teleports;

        if (playerVehicle) {
            if (isStringSpot) {
                // player.sendChat("TP/VEHICLE");

                playerVehicle.setPosition(SPOTS[args]);
            } else if (isNumericCoords) {
                const [x, y, z] = args.slice(0, 3).map(parseFloat);
                playerVehicle.setPosition(sdk.Vector3(x, y, z));
            }
        } else {
            if (isStringSpot) {
                // player.sendChat("TP/PLAYER");

                player.setPosition(SPOTS[args]);
            } else if (isNumericCoords) {
                const [x, y, z] = args.slice(0, 3).map(parseFloat);
                player.setPosition(sdk.Vector3(x, y, z));
            }
        }
    }
};

module.exports = {
    teleport,
};
