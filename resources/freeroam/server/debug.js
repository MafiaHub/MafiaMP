/**
 * Debug event handlers for freeroam gamemode.
 * Only active when ENABLE_DEBUG is true.
 */

import { ENABLE_DEBUG } from "./config.js";

if (ENABLE_DEBUG) {
    // Debug: Log all vehicle streaming events
    Events.on("onStreamIn", (entity) => {
        console.log(`[DEBUG] Entity streamed in: ${entity.id}`);
    });

    Events.on("onStreamOut", (entity) => {
        console.log(`[DEBUG] Entity streamed out: ${entity.id}`);
    });
}
