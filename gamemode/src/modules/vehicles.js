console.log("[GAMEMODE|MODULES] vehicles.js loaded!");

const spawnCars = function (carList) {
    for (const veh of carList) {
        const car = sdk.World.createVehicle(veh.modelName);
        car.setPosition(veh.pos);
        car.setRotation(veh.rot);
    }
    console.log(`[GAMEMODE] spawned ${carList.length} vehicles!`);
};

const setPlates = function (player, plateNumber) {
    const playerVehicle = player.getVehicle();

    if (playerVehicle) {
        playerVehicle.setLicensePlate(plateNumber);
    }
};

module.exports = {
    spawnCars,
    setPlates,
};
