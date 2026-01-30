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
Events.on("vehiclePlayerEnter", (vehicle, player, seatIndex) => {
    console.log(
        `[FREEROAM] Player ${player.nickname} entered vehicle ${vehicle.modelName} (id: ${vehicle.id}) at seat ${seatIndex}.`
    );
    vehicle.engineOn = true;
});

Events.on("vehiclePlayerLeave", (vehicle, player) => {
    console.log(
        `[FREEROAM] Player ${player.nickname} exited vehicle ${vehicle.modelName} (id: ${vehicle.id}).`
    );
    vehicle.engineOn = false;
});

// Player events
Events.on("playerConnect", (player) => {
    console.log(`[FREEROAM] Player ${player.nickname} connected!`);
    Chat.sendToAll(`[SERVER] ${player.nickname} has joined the session!`);

    player.addWeapon(2, 200);
    player.position = SPAWN_POINT.POSITION;
    player.rotation = SPAWN_POINT.ROTATION;
    Chat.sendToPlayer(player, `[SERVER] Welcome ${player.nickname}!`);
});

Events.on("playerDisconnect", (player) => {
    console.log(`[FREEROAM] Player ${player.nickname} disconnected.`);
    Chat.sendToAll(`[SERVER] ${player.nickname} has left the session.`);
});

Events.on("playerDied", (player) => {
    console.log(`[FREEROAM] Player ${player.nickname} died.`);
    Chat.sendToAll(`[SERVER] ${player.nickname} died.`);

    // Reset the player
    player.health = 100.0;
    player.position = SPAWN_POINT.POSITION;
    player.rotation = SPAWN_POINT.ROTATION;
});

// Chat events
Events.on("chatMessage", (player, message) => {
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
    Chat.sendToPlayer(player,`[SERVER] ${player.nickname} triggered a custom event with foo: ${foo}`);
});

// Chat command handler
Events.on("chatCommand", (player, message, command, args) => {
    console.log(`[FREEROAM] Player ${player.nickname} used command: "${command}". (${message}).`);

    const foundCommand = REGISTERED_CHAT_COMMANDS[command];

    if (!foundCommand) {
        Chat.sendToPlayer(player,`[SERVER] Unknown command "${command}".`);
        return;
    }

    foundCommand(player, message, command, args);
});

// ========== CHAT COMMANDS ==========

registerChatCommand("veh", (player, message, command, args) => {
    const modelName = args[0] || "berkley_810";
    const veh = World.createVehicle(modelName);

    if (!veh) {
        Chat.sendToPlayer(player,`[SERVER] Unable to create vehicle ${modelName}.`);
        return;
    }

    veh.position = player.position;
    veh.rotation = player.rotation;

    Chat.sendToPlayer(player,`[SERVER] Vehicle ${modelName} created!`);
});

registerChatCommand("plate", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const licensePlate = args[0] || "";
    veh.licensePlate = licensePlate;

    Chat.sendToPlayer(player,`[SERVER] License plate is now ${licensePlate.length > 0 ? licensePlate : "empty"}!`);
});

registerChatCommand("dirt", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const dirt = parseFloat(args[0]);
    if (isNaN(dirt)) {
        Chat.sendToPlayer(player,"[SERVER] You must provide a dirt value.");
        return;
    }

    veh.dirt = dirt;
    Chat.sendToPlayer(player,`[SERVER] Dirt is now ${dirt}!`);
});

registerChatCommand("radio", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    if (args[0] === undefined) {
        veh.radioOn = !veh.radioOn;
        Chat.sendToPlayer(player,`[SERVER] Radio turned to ${veh.radioOn ? "on" : "off"}!`);
        return;
    }

    const radioStationId = parseInt(args[0]);
    if (isNaN(radioStationId)) {
        Chat.sendToPlayer(player,"[SERVER] Wrong radio id.");
        return;
    }

    veh.radioStationId = radioStationId;
    veh.radioOn = true;

    Chat.sendToPlayer(player,`[SERVER] Radio station is now ${veh.radioStationId}!`);
});

registerChatCommand("rust", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const rust = parseFloat(args[0]);
    if (isNaN(rust)) {
        Chat.sendToPlayer(player,"[SERVER] You must provide a rust value.");
        return;
    }

    veh.rust = rust;
    Chat.sendToPlayer(player,`[SERVER] Rust is now ${rust}!`);
});

registerChatCommand("fuel", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const fuel = parseFloat(args[0]);
    if (isNaN(fuel)) {
        Chat.sendToPlayer(player,"[SERVER] You must provide a fuel value.");
        return;
    }

    veh.fuel = fuel;
    Chat.sendToPlayer(player,`[SERVER] Fuel is now ${fuel}!`);
});

registerChatCommand("colors", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    const colorPrimary = Color.fromRGB(getRandomColor(), getRandomColor(), getRandomColor());
    veh.colorPrimary = colorPrimary;
    Chat.sendToPlayer(player,`[SERVER] Primary color is now ${colorPrimary.r}, ${colorPrimary.g}, ${colorPrimary.b}!`);

    const colorSecondary = Color.fromRGB(getRandomColor(), getRandomColor(), getRandomColor());
    veh.colorSecondary = colorSecondary;
    Chat.sendToPlayer(player,`[SERVER] Secondary color is now ${colorSecondary.r}, ${colorSecondary.g}, ${colorSecondary.b}!`);
});

registerChatCommand("wheelcol", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    const rimColor = Color.fromRGB(getRandomColor(), getRandomColor(), getRandomColor());
    veh.rimColor = rimColor;
    Chat.sendToPlayer(player,`[SERVER] Rim color is now ${rimColor.r}, ${rimColor.g}, ${rimColor.b}!`);

    const tireColor = Color.fromRGB(getRandomColor(), getRandomColor(), getRandomColor());
    veh.tireColor = tireColor;
    Chat.sendToPlayer(player,`[SERVER] Tire color is now ${tireColor.r}, ${tireColor.g}, ${tireColor.b}!`);
});

registerChatCommand("wintint", (player, message, command, args) => {
    const veh = player.vehicle;

    if (!veh) {
        Chat.sendToPlayer(player,"[SERVER] You're not in a vehicle.");
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    const windowTint = Color.fromRGB(getRandomColor(), getRandomColor(), getRandomColor(), getRandomColor());
    veh.windowTint = windowTint;
    Chat.sendToPlayer(player,`[SERVER] Window tint is now ${windowTint.r}, ${windowTint.g}, ${windowTint.b}, ${windowTint.a}!`);
});

registerChatCommand("wep", (player, message, command, args) => {
    const weaponId = parseInt(args[0]) || 85;
    const ammo = parseInt(args[1]) || 200;

    player.addWeapon(weaponId, ammo);
    Chat.sendToPlayer(player,`[SERVER] Weapon ${weaponId} with ${ammo} ammo added!`);
});

registerChatCommand("heal", (player, message, command, args) => {
    player.health = 100.0;
    Chat.sendToPlayer(player,"[SERVER] You've been healed!");
});

registerChatCommand("tp", (player, message, command, args) => {
    const tpDestName = args[0];

    if (!tpDestName) {
        Chat.sendToPlayer(player,"[SERVER] You must provide a teleport destination. (/tp <destination>)");
        Chat.sendToPlayer(player,"Available destinations: " + utils.getObjectKeys(TP_DESTINATIONS).join(", "));
        return;
    }

    const foundTpDest = TP_DESTINATIONS[tpDestName];

    if (!foundTpDest) {
        Chat.sendToPlayer(player,`[SERVER] Unknown teleport destination "${tpDestName}".`);
        return;
    }

    player.position = foundTpDest.POSITION;
    player.rotation = foundTpDest.ROTATION;
    Chat.sendToPlayer(player,`[SERVER] Teleported to ${tpDestName}!`);
});

registerChatCommand("coords", (player, message, command, args) => {
    const x = parseFloat(args[0]);
    const y = parseFloat(args[1]);
    const z = parseFloat(args[2]);

    if (isNaN(x) || isNaN(y) || isNaN(z)) {
        Chat.sendToPlayer(player,"[SERVER] You must provide x, y and z. (/coords <x> <y> <z>)");
        return;
    }

    player.position = new Vector3(x, y, z);
    Chat.sendToPlayer(player,`[SERVER] Teleported to ${x}, ${y}, ${z}!`);
});

// Where am I?
registerChatCommand("wai", (player, message, command, args) => {
    const pos = player.position;
    const rot = player.rotation;

    Chat.sendToPlayer(player,`[SERVER] Your position: ${pos.x}, ${pos.y}, ${pos.z}`);
    Chat.sendToPlayer(player,`[SERVER] Your rotation: ${rot.x}, ${rot.y}, ${rot.z}`);

    // Log in console for easy copy-paste
    console.log(`[FREEROAM] Player ${player.nickname} position: ${pos.x}, ${pos.y}, ${pos.z}`);
    console.log(`[FREEROAM] Player ${player.nickname} rotation: ${rot.x}, ${rot.y}, ${rot.z}`);
});

registerChatCommand("time", (player, message, command, args) => {
    const time = parseFloat(args[0]);

    if (isNaN(time) || time < 0 || time > 24) {
        Chat.sendToPlayer(player,"[SERVER] You must provide time between 0.0 to 24.0. (/time <time>)");
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
    Chat.sendToPlayer(player,`[SERVER] There are ${count} player(s) online.`);

    // List all players using forEach
    World.players.forEach(p => {
        Chat.sendToPlayer(player,`  - ${p.nickname} (id: ${p.id})`);
    });
});

registerChatCommand("vehicles", (player, message, command, args) => {
    const count = World.vehicles.length;
    Chat.sendToPlayer(player,`[SERVER] There are ${count} vehicle(s) in the world.`);

    // Get all vehicle positions using map
    const positions = World.vehicles.map(v => v.position);
    positions.forEach((pos, i) => {
        Chat.sendToPlayer(player,`  Vehicle ${i}: ${pos.x.toFixed(1)}, ${pos.y.toFixed(1)}, ${pos.z.toFixed(1)}`);
    });
});

registerChatCommand("findme", (player, message, command, args) => {
    // Find current player by nickname
    const found = World.players.find(p => p.id === player.id);
    if (found) {
        Chat.sendToPlayer(player,`[SERVER] Found you: ${found.nickname}`);
    } else {
        Chat.sendToPlayer(player,"[SERVER] Could not find you (unexpected!)");
    }
});

registerChatCommand("healall", (player, message, command, args) => {
    // Heal all players using forEach
    World.players.forEach(p => {
        p.health = 100.0;
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

    Chat.sendToPlayer(player,`[SERVER] Found ${nearby.length} vehicle(s) within ${maxDist}m.`);
});

registerChatCommand("anyplayer", (player, message, command, args) => {
    // Check if any player has more than 50 health
    const hasHealthy = World.players.some(p => p.health > 50);
    Chat.sendToPlayer(player,`[SERVER] Any player with >50 health? ${hasHealthy ? "Yes" : "No"}`);
});

registerChatCommand("allhealthy", (player, message, command, args) => {
    // Check if all players have more than 0 health
    const allAlive = World.players.every(p => p.health > 0);
    Chat.sendToPlayer(player,`[SERVER] All players alive? ${allAlive ? "Yes" : "No"}`);
});
