function gamemodeLoaded()
    consoleLog("Gamemode script started")

    -- Create an RGB object
    local color2 = RGB.new(255, 0, 0)

    consoleLog(color2:toString())
    color2:add(0, 255, 0)
    consoleLog(color2:toString())

end

function gamemodeUnloading()
    consoleLog("Gamemode script stopped")
end

function gamemodeUpdated()
    consoleLog("Gamemode script updated")
end