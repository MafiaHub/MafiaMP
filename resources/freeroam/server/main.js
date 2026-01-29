/**
 * Freeroam gamemode - main server script.
 * Handles player spawning, vehicle management, chat commands, and events.
 */

import { SPAWN_POINT, VEHICLE_SPAWNS, WEATHER_SETS, TP_DESTINATIONS } from "./config.js";

// Get utils from shared-utils resource export
const utils = Framework.Exports.get("shared-utils", "utils");

// Load debug handlers
import "./debug.js";

// Chat command registry
const REGISTERED_CHAT_COMMANDS = {};

/**
 * Register a chat command handler.
 * @param {string} name - Command name (without slash)
 * @param {function} handler - Command handler function(player, message, command, args)
 */
function registerChatCommand(name, handler) {
    REGISTERED_CHAT_COMMANDS[name] = handler;
}

// Resource lifecycle
Events.on('resourceStart', (resourceName) => {
    if (resourceName !== 'freeroam') return;

    console.log("[FREEROAM] Resource started!");

    // Spawn vehicles
    for (const v of VEHICLE_SPAWNS) {
        const veh = World.createVehicle(v.MODEL_NAME);
        veh.position = v.POSITION;
        veh.rotation = v.ROTATION;  // Accepts both Vector3 (euler degrees) and Quaternion
    }

    // Set random weather
    const weatherSet = utils.getRandomInArray(WEATHER_SETS);
    if (weatherSet) {
        World.setWeatherSet(weatherSet);
        console.log(`[FREEROAM] Weather set changed to ${weatherSet}.`);
    }
});

Events.on('resourceStop', (resourceName) => {
    if (resourceName !== 'freeroam') return;

    console.log("[FREEROAM] Resource stopping!");
});

// Vehicle events
Events.on("onVehiclePlayerEnter", (vehicle, player, seatIndex) => {
    console.log(
        `[FREEROAM] Player ${player.nickname} entered vehicle ${vehicle.getModelName()} (id: ${vehicle.id}) at seat ${seatIndex}.`
    );
    vehicle.setEngineOn(true);
});

Events.on("onVehiclePlayerLeave", (vehicle, player) => {
    console.log(
        `[FREEROAM] Player ${player.nickname} exited vehicle ${vehicle.getModelName()} (id: ${vehicle.id}).`
    );
    vehicle.setEngineOn(false);
});

// Player events
Events.on("onPlayerConnected", (player) => {
    console.log(`[FREEROAM] Player ${player.nickname} connected!`);
    Chat.sendToAll(`[SERVER] ${player.nickname} has joined the session!`);

    player.addWeapon(2, 200);
    player.position = SPAWN_POINT.POSITION;
    player.rotation = SPAWN_POINT.ROTATION;
    Chat.sendToPlayer(player, `[SERVER] Welcome ${player.nickname}!`);
});

Events.on("onPlayerDisconnected", (player) => {
    console.log(`[FREEROAM] Player ${player.nickname} disconnected.`);
    Chat.sendToAll(`[SERVER] ${player.nickname} has left the session.`);
});

Events.on("onPlayerDied", (player) => {
    console.log(`[FREEROAM] Player ${player.nickname} died.`);
    Chat.sendToAll(`[SERVER] ${player.nickname} died.`);

    // Reset the player
    player.setHealth(100.0);
    player.position = SPAWN_POINT.POSITION;
    player.rotation = SPAWN_POINT.ROTATION;
});

// Chat events
Events.on("onChatMessage", (player, message) => {
    console.log(`[FREEROAM] Player ${player.nickname} said: ${message}`);
    Chat.sendToAll(`<${player.nickname}>: ${message}`);
});

/**
 * Custom event example
 * @param {Player} player
 * @param {string} foo
 */
Events.on("myCustomEvent", (player, foo) => {
    console.log(`[FREEROAM] ${player.nickname} triggered a custom event with foo: ${foo}`);
    player.sendChat(`[SERVER] ${player.nickname} triggered a custom event with foo: ${foo}`);
});

// Chat command handler
Events.on("onChatCommand", (player, message, command, args) => {
    console.log(`[FREEROAM] Player ${player.nickname} used command: "${command}". (${message}).`);

    const foundCommand = REGISTERED_CHAT_COMMANDS[command];

    if (!foundCommand) {
        player.sendChat(`[SERVER] Unknown command "${command}".`);
        return;
    }

    foundCommand(player, message, command, args);
});

// ========== CHAT COMMANDS ==========

registerChatCommand("veh", (player, message, command, args) => {
    const modelName = args[0] || "berkley_810";
    const veh = World.createVehicle(modelName);

    if (!veh) {
        player.sendChat(`[SERVER] Unable to create vehicle ${modelName}.`);
        return;
    }

    veh.position = player.position;
    veh.rotation = player.rotation;

    player.sendChat(`[SERVER] Vehicle ${modelName} created!`);
});

registerChatCommand("plate", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const licensePlate = args[0] || "";
    veh.setLicensePlate(licensePlate);

    player.sendChat(`[SERVER] License plate is now ${licensePlate.length > 0 ? licensePlate : "empty"}!`);
});

registerChatCommand("dirt", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const dirt = parseFloat(args[0]);
    if (isNaN(dirt)) {
        player.sendChat("[SERVER] You must provide a dirt value.");
        return;
    }

    veh.setDirt(dirt);
    player.sendChat(`[SERVER] Dirt is now ${dirt}!`);
});

registerChatCommand("radio", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    if (args[0] === undefined) {
        veh.setRadioOn(!veh.getRadioOn());
        player.sendChat(`[SERVER] Radio turned to ${veh.getRadioOn() ? "on" : "off"}!`);
        return;
    }

    const radioStationId = parseInt(args[0]);
    if (isNaN(radioStationId)) {
        player.sendChat("[SERVER] Wrong radio id.");
        return;
    }

    veh.setRadioStationId(radioStationId);
    veh.setRadioOn(true);

    player.sendChat(`[SERVER] Radio station is now ${veh.getRadioStationId()}!`);
});

registerChatCommand("rust", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const rust = parseFloat(args[0]);
    if (isNaN(rust)) {
        player.sendChat("[SERVER] You must provide a rust value.");
        return;
    }

    veh.setRust(rust);
    player.sendChat(`[SERVER] Rust is now ${rust}!`);
});

registerChatCommand("fuel", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const fuel = parseFloat(args[0]);
    if (isNaN(fuel)) {
        player.sendChat("[SERVER] You must provide a fuel value.");
        return;
    }

    veh.setFuel(fuel);
    player.sendChat(`[SERVER] Fuel is now ${fuel}!`);
});

registerChatCommand("colors", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    const colorPrimary = new Color(getRandomColor(), getRandomColor(), getRandomColor());
    veh.setColorPrimary(colorPrimary);
    player.sendChat(`[SERVER] Primary color is now ${colorPrimary.r}, ${colorPrimary.g}, ${colorPrimary.b}!`);

    const colorSecondary = new Color(getRandomColor(), getRandomColor(), getRandomColor());
    veh.setColorSecondary(colorSecondary);
    player.sendChat(`[SERVER] Secondary color is now ${colorSecondary.r}, ${colorSecondary.g}, ${colorSecondary.b}!`);
});

registerChatCommand("wheelcol", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    const rimColor = new Color(getRandomColor(), getRandomColor(), getRandomColor());
    veh.setRimColor(rimColor);
    player.sendChat(`[SERVER] Rim color is now ${rimColor.r}, ${rimColor.g}, ${rimColor.b}!`);

    const tireColor = new Color(getRandomColor(), getRandomColor(), getRandomColor());
    veh.setTireColor(tireColor);
    player.sendChat(`[SERVER] Tire color is now ${tireColor.r}, ${tireColor.g}, ${tireColor.b}!`);
});

registerChatCommand("wintint", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat("[SERVER] You're not in a vehicle.");
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    const windowTint = new Color(getRandomColor(), getRandomColor(), getRandomColor(), getRandomColor());
    veh.setWindowTint(windowTint);
    player.sendChat(`[SERVER] Window tint is now ${windowTint.r}, ${windowTint.g}, ${windowTint.b}, ${windowTint.a}!`);
});

registerChatCommand("wep", (player, message, command, args) => {
    const weaponId = parseInt(args[0]) || 85;
    const ammo = parseInt(args[1]) || 200;

    player.addWeapon(weaponId, ammo);
    player.sendChat(`[SERVER] Weapon ${weaponId} with ${ammo} ammo added!`);
});

registerChatCommand("heal", (player, message, command, args) => {
    player.setHealth(100.0);
    player.sendChat("[SERVER] You've been healed!");
});

registerChatCommand("tp", (player, message, command, args) => {
    const tpDestName = args[0];

    if (!tpDestName) {
        player.sendChat("[SERVER] You must provide a teleport destination. (/tp <destination>)");
        player.sendChat("Available destinations: " + utils.getObjectKeys(TP_DESTINATIONS).join(", "));
        return;
    }

    const foundTpDest = TP_DESTINATIONS[tpDestName];

    if (!foundTpDest) {
        player.sendChat(`[SERVER] Unknown teleport destination "${tpDestName}".`);
        return;
    }

    player.position = foundTpDest.POSITION;
    player.rotation = foundTpDest.ROTATION;
    player.sendChat(`[SERVER] Teleported to ${tpDestName}!`);
});

registerChatCommand("coords", (player, message, command, args) => {
    const x = parseFloat(args[0]);
    const y = parseFloat(args[1]);
    const z = parseFloat(args[2]);

    if (isNaN(x) || isNaN(y) || isNaN(z)) {
        player.sendChat("[SERVER] You must provide x, y and z. (/coords <x> <y> <z>)");
        return;
    }

    player.position = new Vector3(x, y, z);
    player.sendChat(`[SERVER] Teleported to ${x}, ${y}, ${z}!`);
});

// Where am I?
registerChatCommand("wai", (player, message, command, args) => {
    const pos = player.position;
    const rot = player.rotation;

    player.sendChat(`[SERVER] Your position: ${pos.x}, ${pos.y}, ${pos.z}`);
    player.sendChat(`[SERVER] Your rotation: ${rot.x}, ${rot.y}, ${rot.z}`);

    // Log in console for easy copy-paste
    console.log(`[FREEROAM] Player ${player.nickname} position: ${pos.x}, ${pos.y}, ${pos.z}`);
    console.log(`[FREEROAM] Player ${player.nickname} rotation: ${rot.x}, ${rot.y}, ${rot.z}`);
});

registerChatCommand("time", (player, message, command, args) => {
    const time = parseFloat(args[0]);

    if (isNaN(time) || time < 0 || time > 24) {
        player.sendChat("[SERVER] You must provide time between 0.0 to 24.0. (/time <time>)");
        return;
    }

    World.setDayTimeHours(time);
});

registerChatCommand("customevent", (player, message, command, args) => {
    Events.emit("myCustomEvent", player, "bar");
});

// ========== COLLECTION TEST COMMANDS ==========

registerChatCommand("players", (player, message, command, args) => {
    const count = World.players.length;
    player.sendChat(`[SERVER] There are ${count} player(s) online.`);

    // List all players using forEach
    World.players.forEach(p => {
        player.sendChat(`  - ${p.nickname} (id: ${p.id})`);
    });
});

registerChatCommand("vehicles", (player, message, command, args) => {
    const count = World.vehicles.length;
    player.sendChat(`[SERVER] There are ${count} vehicle(s) in the world.`);

    // Get all vehicle positions using map
    const positions = World.vehicles.map(v => v.position);
    positions.forEach((pos, i) => {
        player.sendChat(`  Vehicle ${i}: ${pos.x.toFixed(1)}, ${pos.y.toFixed(1)}, ${pos.z.toFixed(1)}`);
    });
});

registerChatCommand("findme", (player, message, command, args) => {
    // Find current player by nickname
    const found = World.players.find(p => p.id === player.id);
    if (found) {
        player.sendChat(`[SERVER] Found you: ${found.nickname}`);
    } else {
        player.sendChat("[SERVER] Could not find you (unexpected!)");
    }
});

registerChatCommand("healall", (player, message, command, args) => {
    // Heal all players using forEach
    World.players.forEach(p => {
        p.setHealth(100.0);
    });
    Chat.sendToAll("[SERVER] All players have been healed!");
});

registerChatCommand("nearbyveh", (player, message, command, args) => {
    const maxDist = parseFloat(args[0]) || 50.0;
    const myPos = player.position;

    // Filter vehicles within distance
    const nearby = World.vehicles.filter(v => {
        const vPos = v.position;
        const dx = vPos.x - myPos.x;
        const dy = vPos.y - myPos.y;
        const dz = vPos.z - myPos.z;
        return Math.sqrt(dx*dx + dy*dy + dz*dz) <= maxDist;
    });

    player.sendChat(`[SERVER] Found ${nearby.length} vehicle(s) within ${maxDist}m.`);
});

registerChatCommand("anyplayer", (player, message, command, args) => {
    // Check if any player has more than 50 health
    const hasHealthy = World.players.some(p => p.getHealth() > 50);
    player.sendChat(`[SERVER] Any player with >50 health? ${hasHealthy ? "Yes" : "No"}`);
});

registerChatCommand("allhealthy", (player, message, command, args) => {
    // Check if all players have more than 0 health
    const allAlive = World.players.every(p => p.getHealth() > 0);
    player.sendChat(`[SERVER] All players alive? ${allAlive ? "Yes" : "No"}`);
});
