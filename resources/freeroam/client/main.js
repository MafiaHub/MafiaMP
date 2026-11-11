/**
 * Freeroam gamemode - client script.
 */

console.log("[FREEROAM] Client resource started!");

// ========== CHARACTER CREATOR (/creator) ==========

const creator = Web.createView("fw://resources/freeroam/client/ui/outfit.html", { width: 0, height: 0, zIndex: 3, visible: false });
let creatorOpen = false;

function setCreatorOpen(open) {
    creatorOpen = open;
    if (open) {
        Web.showView(creator);
        Web.focusView(creator, true);
    } else {
        Web.focusView(creator, false);
        Web.hideView(creator);
    }
}

Events.on("freeroam:openCreator", () => setCreatorOpen(true));
Web.on(creator, "outfit:close", () => setCreatorOpen(false));
Web.on(creator, "outfit:apply", (payload) => {
    Events.emitServer("freeroam:outfit", { names: (payload && payload.names) || [] });
});

Events.on("browserConsoleMessage", ({ viewId, message, severity }) => {
    if (viewId === creator && severity === "error") {
        console.log(`[FREEROAM] Creator: ${message}`);
    }
});
