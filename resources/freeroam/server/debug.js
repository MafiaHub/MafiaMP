/**
 * Debug event handlers for freeroam gamemode.
 * Only active when ENABLE_DEBUG is true.
 */

import { ENABLE_DEBUG } from "./config.js";

if (ENABLE_DEBUG) {
    Framework.events.on("onPlayerConnected", (player) => {
        console.log("[DEBUG][onPlayerConnected] player:", player.toString());
        console.log("[DEBUG][onPlayerConnected] player id:", player.id, "| type:", typeof player.id);
        console.log("[DEBUG][onPlayerConnected] player name:", player.name, "| type:", typeof player.name);
        console.log("[DEBUG][onPlayerConnected] player nickname:", player.nickname, "| type:", typeof player.nickname);
        console.log("[DEBUG][onPlayerConnected] player modelHash:", player.getModelHash(), "| type:", typeof player.getModelHash());
        console.log("[DEBUG][onPlayerConnected] player modelName:", player.getModelName(), "| type:", typeof player.getModelName());
    });

    Framework.events.on("onVehiclePlayerEnter", (vehicle, player, seatIndex) => {
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle:", vehicle.toString());
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle id:", vehicle.id, "| type:", typeof vehicle.id);
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle name:", vehicle.name, "| type:", typeof vehicle.name);
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle nickname:", vehicle.nickname, "| type:", typeof vehicle.nickname);
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle player seat index:", seatIndex, "| type:", typeof seatIndex);
        console.log("[DEBUG][onVehiclePlayerEnter] player id:", player.id, "| type:", typeof player.id);
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle modelHash:", vehicle.getModelHash(), "| type:", typeof vehicle.getModelHash());
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle modelName:", vehicle.getModelName(), "| type:", typeof vehicle.getModelName());
        console.log("[DEBUG][onVehiclePlayerEnter] vehicle plate:", vehicle.getLicensePlate(), "| type:", typeof vehicle.getLicensePlate());
    });

    Framework.events.on("onChatCommand", (player, message, command, args) => {
        console.log("[DEBUG][onChatCommand] owner:", player.nickname);
        console.log("[DEBUG][onChatCommand] message:", message);
        console.log("[DEBUG][onChatCommand] command:", command);
        console.log("[DEBUG][onChatCommand] args:", args.join(", "), "| type:", typeof args, "| length:", args.length);
    });
}
