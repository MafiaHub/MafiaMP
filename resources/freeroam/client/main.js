/**
 * Freeroam gamemode - client script.
 */

function onResourceStart() {
    console.log("[FREEROAM] Client resource started!");
}

function onResourceStop() {
    console.log("[FREEROAM] Client resource stopping!");
}

module.exports = {
    onResourceStart,
    onResourceStop
};
