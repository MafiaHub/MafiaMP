const vehicleSpawns = [
    {
        modelName: "berkley_810",
        pos: sdk.Vector3(-986.40686, -304.061798, 2.292042),
        rot: sdk.Quaternion(0.70629, 0.006456, -0.004993, -0.707875),
    },
    {
        modelName: "bolt_ace",
        pos: sdk.Vector3(-985.365356, -336.348083, 2.892426),
        rot: sdk.Quaternion(0.702591, 0.00959, 0.005905, -0.711505),
    },
    {
        modelName: "bolt_ace_pickup",
        pos: sdk.Vector3(-986.426086, -343.213989, 2.942883),
        rot: sdk.Quaternion(0.734178, 0.009719, 0.005454, -0.678866),
    },
    {
        modelName: "bolt_cooler",
        pos: sdk.Vector3(-986.180481, -346.679779, 2.86176),
        rot: sdk.Quaternion(0.699282, 0.007726, 0.000273, -0.714804),
    },
    {
        modelName: "bolt_delivery",
        pos: sdk.Vector3(-986.150085, -349.681244, 2.773793),
        rot: sdk.Quaternion(0.707309, 0.008673, 0.002028, -0.706848),
    },
    {
        modelName: "bolt_delivery_amb",
        pos: sdk.Vector3(-986.273193, -352.752472, 2.80282),
        rot: sdk.Quaternion(0.722713, 0.008347, 0.001777, -0.691096),
    },
    {
        modelName: "bolt_hearse",
        pos: sdk.Vector3(-1018.279968, -340.606567, 2.919076),
        rot: sdk.Quaternion(0.72342, 0.023575, -0.020525, 0.689701),
    },
    {
        modelName: "bolt_mail",
        pos: sdk.Vector3(-1015.080139, -344.603149, 2.729358),
        rot: sdk.Quaternion(0.70901, 0.019996, -0.011678, 0.704818),
    },
    {
        modelName: "bolt_model_b",
        pos: sdk.Vector3(-1014.999268, -347.786743, 2.713083),
        rot: sdk.Quaternion(0.701918, 0.016684, -0.009163, 0.712003),
    },
    {
        modelName: "bolt_pickup",
        pos: sdk.Vector3(-1014.623901, -350.718506, 2.802622),
        rot: sdk.Quaternion(0.688722, 0.015586, -0.009113, 0.7248),
    },
    {
        modelName: "bolt_truck",
        pos: sdk.Vector3(-984.092224, -395.968231, 3.501376),
        rot: sdk.Quaternion(0.716167, 0.003727, 0.00603, -0.697893),
    },
    {
        modelName: "bolt_v8",
        pos: sdk.Vector3(-1014.788574, -353.798706, 2.868016),
        rot: sdk.Quaternion(0.707765, 0.029924, 0.002543, 0.705809),
    },
    {
        modelName: "brubaker_forte",
        pos: sdk.Vector3(-1111.575073, -235.930466, 2.627016),
        rot: sdk.Quaternion(0.999993, 0.000999, -0.001624, -0.003262),
    },
    {
        modelName: "bulworth_packhard",
        pos: sdk.Vector3(-1128.774658, -210.558975, 2.788571),
        rot: sdk.Quaternion(0.018355, -0.000992, 0.004058, -0.999823),
    },
    {
        modelName: "bulworth_sentry",
        pos: sdk.Vector3(-1132.61499, -211.015091, 2.987224),
        rot: sdk.Quaternion(0.005594, 0.00386, -0.007584, 0.999948),
    },
    {
        modelName: "carrozella_c_series",
        pos: sdk.Vector3(-1114.119995, -235.950943, 2.574988),
        rot: sdk.Quaternion(0.999987, 0.000122, -0.001638, 0.004916),
    },
    {
        modelName: "celeste_mark_5",
        pos: sdk.Vector3(-1128.14563, -235.091614, 2.74097),
        rot: sdk.Quaternion(0.999955, 0.00423, 0.002423, -0.008165),
    },
    {
        modelName: "culver_airmaster",
        pos: sdk.Vector3(-1131.364136, -234.9207, 2.780458),
        rot: sdk.Quaternion(0.999437, 0.00704, -0.001867, -0.032764),
    },
    {
        modelName: "eckhart_crusader",
        pos: sdk.Vector3(-1134.74292, -234.785156, 2.649705),
        rot: sdk.Quaternion(0.999528, 0.007318, 0.001145, -0.029805),
    },
    {
        modelName: "eckhart_elite",
        pos: sdk.Vector3(-1138.363647, -234.632172, 2.741032),
        rot: sdk.Quaternion(0.999839, 0.008441, -0.004551, -0.015151),
    },
    {
        modelName: "eckhart_fletcher",
        pos: sdk.Vector3(-1124.938232, -234.045761, 2.648867),
        rot: sdk.Quaternion(0.999995, 0.00146, -0.002389, -0.001145),
    },
    {
        modelName: "falconer_classic",
        pos: sdk.Vector3(-1135.854736, -210.904358, 2.817213),
        rot: sdk.Quaternion(0.002454, -0.007236, 0.003241, -0.999966),
    },
    {
        modelName: "houston_coupe",
        pos: sdk.Vector3(-914.322388, -120.731689, 3.724135),
        rot: sdk.Quaternion(0.722015, -0.010507, -0.006834, 0.691764),
    },
    {
        modelName: "lassiter_v16",
        pos: sdk.Vector3(-914.743103, -117.816315, 3.732256),
        rot: sdk.Quaternion(0.727089, -0.011908, -0.007143, 0.686403),
    },
    {
        modelName: "lassiter_v16_appolyon",
        pos: sdk.Vector3(-915.122864, -114.279373, 3.828074),
        rot: sdk.Quaternion(0.695113, -0.010159, -0.007071, 0.718794),
    },
    {
        modelName: "lassiter_v16_roadster",
        pos: sdk.Vector3(-914.985962, -111.23259, 3.845985),
        rot: sdk.Quaternion(0.718718, -0.011091, -0.002821, 0.695208),
    },
    {
        modelName: "parry_bus",
        pos: sdk.Vector3(-962.758606, -65.251747, 4.112756),
        rot: sdk.Quaternion(0.703505, 0.007003, 0.002187, -0.710653),
    },
    {
        modelName: "samson_drifter",
        pos: sdk.Vector3(-915.068787, -108.109604, 3.96198),
        rot: sdk.Quaternion(0.729244, -0.006566, -0.005744, 0.684198),
    },
    {
        modelName: "samson_tanker",
        pos: sdk.Vector3(-963.507813, -59.896549, 3.865996),
        rot: sdk.Quaternion(0.703396, 0.00494, 0.009704, -0.710714),
    },
    {
        modelName: "shubert_e_six",
        pos: sdk.Vector3(-970.537109, -97.723427, 3.664779),
        rot: sdk.Quaternion(0.999958, -0.004803, 0.006743, 0.003864),
    },
    {
        modelName: "shubert_e_six_p",
        pos: sdk.Vector3(-973.756897, -98.136681, 3.633013),
        rot: sdk.Quaternion(0.999966, -0.004319, 0.006257, -0.003289),
    },
    {
        modelName: "shubert_e_six_taxi",
        pos: sdk.Vector3(-977.302856, -98.2183, 3.608772),
        rot: sdk.Quaternion(0.99997, -0.002881, 0.004691, -0.005354),
    },
    {
        modelName: "shubert_frigate",
        pos: sdk.Vector3(-980.90509, -97.99614, 3.381163),
        rot: sdk.Quaternion(0.999725, -0.008196, 0.005107, -0.021349),
    },
    {
        modelName: "shubert_six",
        pos: sdk.Vector3(-984.109741, -98.175415, 3.511434),
        rot: sdk.Quaternion(0.99994, -0.005686, 0.005481, -0.007586),
    },
    {
        modelName: "shubert_six_det",
        pos: sdk.Vector3(-986.901978, -98.312843, 3.462155),
        rot: sdk.Quaternion(0.999794, -0.005663, 0.006068, -0.018523),
    },
    {
        modelName: "shubert_six_p",
        pos: sdk.Vector3(-990.067322, -98.177017, 3.464381),
        rot: sdk.Quaternion(0.999615, -0.00516, 0.006077, 0.026569),
    },
    {
        modelName: "shubert_six_taxi",
        pos: sdk.Vector3(-993.492859, -98.207672, 3.423491),
        rot: sdk.Quaternion(0.999974, -0.005231, 0.004, -0.00296),
    },
    {
        modelName: "smith_moray",
        pos: sdk.Vector3(-996.777039, -98.042068, 3.426675),
        rot: sdk.Quaternion(0.999928, -0.002293, 0.010735, 0.004858),
    },
    {
        modelName: "smith_thrower",
        pos: sdk.Vector3(-1001.699707, -98.202988, 3.406393),
        rot: sdk.Quaternion(0.999839, 0.007582, 0.005775, 0.015234),
    },
    {
        modelName: "smith_v12",
        pos: sdk.Vector3(-1005.234802, -98.821114, 3.299397),
        rot: sdk.Quaternion(0.99998, -0.001132, 0.005176, 0.003361),
    },
    {
        modelName: "smith_v12_chicago",
        pos: sdk.Vector3(-1009.044983, -98.970703, 3.265859),
        rot: sdk.Quaternion(0.999996, -0.002558, 0.000828, 0.0),
    },
    {
        modelName: "trautenberg_sport",
        pos: sdk.Vector3(-1012.573914, -98.974365, 3.331904),
        rot: sdk.Quaternion(0.999934, -0.001642, 0.00515, 0.010147),
    },
    {
        modelName: "crazy_horse",
        pos: sdk.Vector3(-934.128235, -234.337341, 2.938883),
        rot: sdk.Quaternion(0.99987, 0.016064, -0.000752, -0.001326),
    },
    {
        modelName: "disorder",
        pos: sdk.Vector3(-975.474548, -236.047623, 3.025368),
        rot: sdk.Quaternion(0.700711, 0.001802, -0.001995, 0.71344),
    },
    {
        modelName: "flame_spear",
        pos: sdk.Vector3(-974.755005, -233.292252, 2.886548),
        rot: sdk.Quaternion(0.715502, -0.000532, -0.000854, 0.69861),
    },
    {
        modelName: "manta_prototype",
        pos: sdk.Vector3(-970.275513, -228.638977, 2.805085),
        rot: sdk.Quaternion(0.404505, -0.000488, 0.000977, 0.914535),
    },
    {
        modelName: "mutagen",
        pos: sdk.Vector3(-968.035583, -239.757065, 3.011281),
        rot: sdk.Quaternion(0.89056, -0.003047, -0.0, 0.454856),
    },
    {
        modelName: "hank_a",
        pos: sdk.Vector3(-876.823364, -179.889084, 3.178279),
        rot: sdk.Quaternion(0.699958, 0.010274, 0.009395, -0.714049),
    },
    {
        modelName: "shubert_e_six",
        pos: sdk.Vector3(-906.617432, -193.383072, 2.961988),
        rot: sdk.Quaternion(0.020849, -0.03458, 0.001879, -0.999183),
    },
    {
        modelName: "bolt_v8",
        pos: sdk.Vector3(-906.344666, -155.046341, 3.101451),
        rot: sdk.Quaternion(0.015857, 0.03947, -0.007466, 0.999067),
    },
    {
        modelName: "bolt_model_b",
        pos: sdk.Vector3(-906.452148, -242.218521, 2.937189),
        rot: sdk.Quaternion(0.002211, -0.032577, -0.004167, -0.999458),
    },
    {
        modelName: "shubert_six_det",
        pos: sdk.Vector3(-890.745178, -221.91925, 2.894908),
        rot: sdk.Quaternion(0.999474, -0.003435, 0.027849, -0.016274),
    },
    {
        modelName: "culver_airmaster",
        pos: sdk.Vector3(-890.607422, -199.794052, 2.993601),
        rot: sdk.Quaternion(0.999442, 0.001434, 0.033379, 0.000244),
    },
    {
        modelName: "houston_coupe",
        pos: sdk.Vector3(-890.751465, -147.323196, 3.245617),
        rot: sdk.Quaternion(0.999277, -0.011216, 0.034156, 0.012345),
    },
];

const weatherSets = ["mm_030_molotov_cp_010_cine", "mm_150_boat_cp_010", "mm_210_gallery_cp_050"];

const SPAWN_POINT = {
    pos: sdk.Vector3(-985.871, -299.401, 2.1),
    rot: sdk.Quaternion(0.291, 0, 0, -0.957),
};

sdk.on("gamemodeLoaded", () => {
    console.log("[GAMEMODE] Gamemode loaded!");

    // Spawn vehicles
    for (const veh of vehicleSpawns) {
        const car = sdk.World.createVehicle(veh.modelName);
        car.setPosition(veh.pos);
        car.setRotation(veh.rot);
    }
    console.log(`[GAMEMODE] spawned ${vehicleSpawns.length} vehicles!`);

    // Weather
    const selectedSet = weatherSets[Math.floor(Math.random() * weatherSets.length)];
    sdk.Environment.setWeather(selectedSet);

    // Clock
    let actualTime = 11;
    // Periodically update the time
    setInterval(function () {
        sdk.Environment.setDayTimeHours(actualTime);
        actualTime = (actualTime + 1) % 24;
    }, 1000);
});

sdk.on("gamemodeUnloading", () => {
    console.log("[GAMEMODE] Gamemode unloading!");
});

sdk.on("vehiclePlayerEnter", (vehicle, player, seatIndex) => {
    console.log(`[GAMEMODE] Player ${player.nickname} entered vehicle ${vehicle.name} = ${seatIndex}!`);
    vehicle.setEngineOn(true);
});

sdk.on("vehiclePlayerLeave", (vehicle, player) => {
    console.log(`[GAMEMODE] Player ${player.nickname} exited vehicle ${vehicle.name}!`);
    vehicle.setEngineOn(false);
});

sdk.on("playerConnected", (player) => {
    console.log(`[GAMEMODE] Player ${player.nickname} connected!`);
    player.sendChatToAll(`[SERVER] ${player.nickname} has joined the session!`);

    player.addWeapon(2, 200);
    player.setPosition(SPAWN_POINT.pos);
    player.setRotation(SPAWN_POINT.rot);
    player.sendChat(`[SERVER] Welcome ${player.nickname}!`);
});

sdk.on("playerDisconnected", (player) => {
    console.log(`[GAMEMODE] Player ${player.nickname} disconnected!`);
    player.sendChatToAll(`[SERVER] ${player.nickname} has left the session!`);
});

sdk.on("playerDied", (player) => {
    console.log(`[GAMEMODE] Player ${player.nickname} died!`);
    player.sendChatToAll(`[SERVER] Player ${player.nickname} died!`);

    // Respawn the player
    player.setHealth(100.0);
    player.setPosition(SPAWN_POINT.pos);
    player.setRotation(SPAWN_POINT.rot);
});

sdk.on("chatMessage", (player, message) => {
    console.log(`[GAMEMODE] Player ${player.nickname} said: ${message}`);
    sdk.Chat.sendToAll(`<${player.nickname}>: ${message}`);
});

const REGISTERED_CHAT_COMMANDS = new Map();

sdk.on("chatCommand", (player, message, command, args) => {
    console.log(`[GAMEMODE] Player ${player.nickname} used: ${command}. (${message})`);

    const foundCommand = REGISTERED_CHAT_COMMANDS.get(command);

    if (!foundCommand) {
        sdk.Chat.sendToPlayer(player, `[SERVER] Unknown command "${command}"`);
        return;
    }

    foundCommand(player, message, command, args);
});

function RegisterChatCommand(name, handler) {
    REGISTERED_CHAT_COMMANDS.set(name, handler);
}

RegisterChatCommand("showArgs", (player, message, command, args) => {
    sdk.Chat.sendToAll(`[SERVER] Player ${player.nickname} used /showArgs with args: ${args}`);
});

RegisterChatCommand("heal", (player, message, command, args) => {
    player.setHealth(100);
    player.sendChat(`[SERVER] Health restored!`);
});

const TP_DESTINATIONS = {
    spawn: SPAWN_POINT,
    start: {
        pos: sdk.Vector3(-916.0, -210.0, 2.605),
        rot: sdk.Quaternion(0.707, 0.0, 0.0, 0.707),
    },
    autodrome: {
        pos: sdk.Vector3(-1915.627, 10.715, 17.566),
        rot: sdk.Quaternion(0.99, 0.0, 0.0, 0.139),
    },
};

RegisterChatCommand("tp", (player, message, command, args) => {
    const tpDestName = args[0];
    const foundTpDest = TP_DESTINATIONS[tpDestName];

    if (!foundTpDest) {
        player.sendChat(`[SERVER] Teleportation failed. Unknown destination ${tpDestName}`);
        return;
    }

    player.setPosition(foundTpDest.pos);
    player.setRotation(foundTpDest.rot);
    player.sendChat(`[SERVER] Teleported to ${tpDestName}!`);
});

RegisterChatCommand("coords", (player, message, command, args) => {
    let x = args[0];
    let y = args[1];
    let z = args[2];

    if (x == null || y == null || z == null) {
        player.sendChat(`[SERVER] You must provide x, y and z!`);
        return;
    }

    x = parseFloat(x);
    y = parseFloat(y);
    z = parseFloat(z);

    player.setPosition(sdk.Vector3(x, y, z));
    player.sendChat(`[SERVER] Teleported to ${x}, ${y}, ${z}!`);
});

RegisterChatCommand("pos", (player, message, command, args) => {
    const pos = player.getPosition();
    const rot = player.getRotation();

    const parse = (x) =>
        parseFloat(x)
            .toFixed(3)
            .replace(/\.?0+$/, "");

    const posParsedMessage = `${parse(pos.x)}, ${parse(pos.y)}, ${parse(pos.z)}`;
    const rotParsedMessage = `${parse(rot.w)}, ${parse(rot.x)}, ${parse(rot.y)}, ${parse(rot.z)}`;

    player.sendChat(`[SERVER] Position: ${posParsedMessage}`);
    player.sendChat(`[SERVER] Rotation: ${rotParsedMessage}`);

    // Log in console for easy copy-paste
    console.log(`[GAMEMODE] Player Position: ${posParsedMessage}`);
    console.log(`[GAMEMODE] Player Rotation: ${rotParsedMessage}`);
});

RegisterChatCommand("veh", (player, message, command, args) => {
    const modelName = args[0] ?? "berkley_810";
    const veh = sdk.World.createVehicle(modelName);

    if (!veh) {
        // Condition doesn't works well yet, still an object even if the vehicle doesn't exist
        player.sendChat(`[SERVER] Unable to create vehicle ${modelName}!`);
        return;
    }

    veh.setPosition(player.getPosition());
    veh.setRotation(player.getRotation());

    player.sendChat(`[SERVER] Vehicle ${modelName} created!`);
});

RegisterChatCommand("vehid", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    player.sendChat(`[SERVER] Vehicle id: ${veh.id}, name: ${veh.name}!`);
});

RegisterChatCommand("plate", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const licensePlate = args[0];
    veh.setLicensePlate(licensePlate);

    const newLicensePlate = veh.getLicensePlate();
    player.sendChat(`[SERVER] License plate is ${newLicensePlate.length > 0 ? newLicensePlate : "empty"}!`);
});

RegisterChatCommand("dirt", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const dirt = parseFloat(args[0]);
    if (isNaN(dirt)) {
        player.sendChat(`[SERVER] You must provide a dirt value!`);
        return;
    }

    veh.setDirt(dirt);
    player.sendChat(`[SERVER] Dirt changed to ${veh.getDirt()}!`);
});

RegisterChatCommand("radio", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    if (args[0] == null) {
        veh.setRadioOn(!veh.getRadioOn());
        player.sendChat(`[SERVER] Radio turned to ${veh.getRadioOn() ? "on" : "off"}!`);
        return;
    }

    const radioStationId = parseInt(args[0], 10);
    if (isNaN(radioStationId)) {
        player.sendChat(`[SERVER] Wrong radio id.`);
        return;
    }

    veh.setRadioStationId(radioStationId);
    veh.setRadioOn(true);
    player.sendChat(`[SERVER] Radio station setted to: ${veh.getRadioStationId()}!`);
});

RegisterChatCommand("rust", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const rust = parseFloat(args[0]);
    if (isNaN(rust)) {
        player.sendChat(`[SERVER] You must provide a rust value!`);
        return;
    }

    veh.setRust(rust);
    player.sendChat(`[SERVER] Rust changed to ${veh.getRust()}!`);
});

RegisterChatCommand("fuel", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const fuel = parseFloat(args[0]);
    if (isNaN(fuel)) {
        player.sendChat(`[SERVER] You must provide a fuel value!`);
        return;
    }

    veh.setFuel(fuel);
    player.sendChat(`[SERVER] Fuel changed to ${veh.getFuel()}!`);
});

RegisterChatCommand("colors", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);
    const wantedColorPrimary = sdk.RGB(getRandomColor(), getRandomColor(), getRandomColor());
    const wantedColorSecondary = sdk.RGB(getRandomColor(), getRandomColor(), getRandomColor());

    veh.setColorPrimary(wantedColorPrimary);
    veh.setColorSecondary(wantedColorSecondary);

    const colorPrimary = veh.getColorPrimary();
    player.sendChat(`[SERVER] Primary color changed to rgb: ${colorPrimary.r}, ${colorPrimary.g}, ${colorPrimary.b}`);

    const colorSecondary = veh.getColorSecondary();
    player.sendChat(
        `[SERVER] Secondary color changed to rgb: ${colorSecondary.r}, ${colorSecondary.g}, ${colorSecondary.b}`
    );
});

RegisterChatCommand("wheelcol", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);
    const wantedRimColor = sdk.RGB(getRandomColor(), getRandomColor(), getRandomColor());
    const wantedTireColor = sdk.RGB(getRandomColor(), getRandomColor(), getRandomColor());

    veh.setRimColor(wantedRimColor);
    veh.setTireColor(wantedTireColor);

    const rimColor = veh.getRimColor();
    player.sendChat(`[SERVER] Rim color changed to rgb: ${rimColor.r}, ${rimColor.g}, ${rimColor.b}`);

    const tireColor = veh.getTireColor();
    player.sendChat(`[SERVER] Tire color changed to rgb: ${tireColor.r}, ${tireColor.g}, ${tireColor.b}`);
});

RegisterChatCommand("wintint", (player, message, command, args) => {
    const veh = player.getVehicle();

    if (!veh) {
        player.sendChat(`[SERVER] You're not in a vehicle!`);
        return;
    }

    const getRandomColor = () => Math.floor(Math.random() * 256);

    veh.setWindowTint(sdk.RGBA(getRandomColor(), getRandomColor(), getRandomColor(), getRandomColor()));

    const windowTint = veh.getWindowTint();
    player.sendChat(
        `[SERVER] Window tint changed to rgba: ${windowTint.r}, ${windowTint.g}, ${windowTint.b}, ${windowTint.a}`
    );
});

RegisterChatCommand("wep", (player, message, command, args) => {
    // TODO: doesn't works yet

    const weaponId = parseInt(args[0], 10) ?? 85;
    player.addWeapon(weaponId, 200);
    player.sendChat(`[SERVER] Weapon (id: ${weaponId}) received!`);
});
