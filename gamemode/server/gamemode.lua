listenEvent("onGamemodeLoaded", function ()
    Console:log("Gamemode script started")

    -- Create an RGB object
    local color2 = RGB.new(255, 0, 0)

    Console:log(color2:toString())
    color2:add(0, 255, 0)
    Console:log(color2:toString())
end)

listenEvent("onGamemodeUnloading", function()
    Console:log("Gamemode script stopped")
end)

listenEvent("onGamemodeUpdated", function()
    -- Console:log("Gamemode script updated")
end)

listenEvent("onPlayerConnected", function(player)
    Console:log("Player connected lol")
end)

listenEvent("onChatMessage", function(player, message)
    Console:log("Received message: " .. message)
    Chat:sendToAll(message)
    Console:log("Forwarded message to all: " .. message)
end)
