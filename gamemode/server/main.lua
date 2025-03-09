local CONFIG = require("gamemode/server/config")
local utils = require("gamemode/shared/utils")

require("gamemode/server/debug")

Event.on("onGamemodeLoaded", function ()
  Console.log("[GAMEMODE] Gamemode loaded!")

  -- Spawn vehicles
  for _, v in ipairs(CONFIG.VEHICLE_SPAWNS) do
    local veh = World.createVehicle(v.MODEL_NAME)
    veh:setPosition(v.POSITION)
    veh:setRotation(v.ROTATION)
  end

  -- WeatherSet
  local weatherSet = utils.getRandomInTable(CONFIG.WEATHER_SETS)
  if (weatherSet) then
    World.setWeatherSet(weatherSet)
    Console.log("[GAMEMODE] Weather set changed to " .. weatherSet .. ".")
  end
end)

Event.on("onGamemodeUnloading", function ()
  Console.log("[GAMEMODE] Gamemode unloading!")
end)

Event.on("onVehiclePlayerEnter", function (vehicle, player, seatIndex)
  Console.log(
    "[GAMEMODE] Player " .. player.nickname ..
    " entered vehicle " .. vehicle:getModelName() .. " (id: " .. vehicle.id .. ") at seat " .. seatIndex .. "."
  )

  vehicle:setEngineOn(true)
end)

Event.on("onVehiclePlayerLeave", function (vehicle, player)
  Console.log(
    "[GAMEMODE] Player " .. player.nickname ..
    " exited vehicle " .. vehicle:getModelName() .. " (id: " .. vehicle.id .. ")."
  )

  vehicle:setEngineOn(false)
end)

Event.on("onPlayerConnected", function (player)
  Console.log("[GAMEMODE] Player " .. player.nickname .. " connected!")
  player:sendChatToAll("[SERVER] " .. player.nickname .. " has joined the session!")

  player:addWeapon(2, 200)
  player:setPosition(CONFIG.SPAWN_POINT.POSITION)
  player:setRotation(CONFIG.SPAWN_POINT.ROTATION)
  player:sendChat("[SERVER] Welcome " .. player.nickname .. "!")
end)

Event.on("onPlayerDisconnected", function (player)
  Console.log("[GAMEMODE] Player " .. player.nickname .. " disconnected.")
  player:sendChatToAll("[SERVER] " .. player.nickname .. " has left the session.")
end)

Event.on("onPlayerDied", function (player)
  Console.log("[GAMEMODE] Player " .. player.nickname .. " died.")
  player:sendChatToAll("[SERVER] " .. player.nickname .. " died.")

  -- Reset the player
  player:setHealth(100.0)
  player:setPosition(CONFIG.SPAWN_POINT.POSITION)
  player:setRotation(CONFIG.SPAWN_POINT.ROTATION)
end)

Event.on("onChatMessage", function (player, message)
  Console.log("[GAMEMODE] Player " .. player.nickname .. " said: " .. message)
  Chat.sendToAll("<" .. player.nickname .. ">: " .. message)
end)

---@param player Player
Event.on("myCustomEvent", function (player)
  Console.log("[GAMEMODE] " .. player.nickname .. " triggered a custom event!")
end)

---@type { [string]: OnChatCommandCallback}
local REGISTERED_CHAT_COMMANDS = {}

---@param name string
---@param handler OnChatCommandCallback
function RegisterChatCommand (name, handler)
  REGISTERED_CHAT_COMMANDS[name] = handler
end

Event.on("onChatCommand", function (player, message, command, args)
  Console.log("[GAMEMODE] Player " .. player.nickname .. " used command: \"" .. command .. "\". (" .. message .. ").")

  local foundCommand = REGISTERED_CHAT_COMMANDS[command]

  if (foundCommand == nil) then
    player:sendChat("[SERVER] Unknown command \"" .. command .. "\".")
    return
  end

  foundCommand(player, message, command, args)
end)

RegisterChatCommand("veh", function (player, message, command, args)
  local modelName = args[1] or "berkley_810"
  local veh = World.createVehicle(modelName)

  if veh == nil then
    player:sendChat("[SERVER] Unable to create vehicle " .. modelName .. ".")
    return
  end

  veh:setPosition(player:getPosition())
  veh:setRotation(player:getRotation())

  player:sendChat("[SERVER] Vehicle " .. modelName .. " created!")
end)

RegisterChatCommand("plate", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local licensePlate = args[1] or ""
  veh:setLicensePlate(licensePlate)

  player:sendChat("[SERVER] License plate is now " .. (#licensePlate > 0 and licensePlate or "empty") .. "!")
end)

RegisterChatCommand("dirt", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local dirt = tonumber(args[1])
  if (dirt == nil) then
    player:sendChat("[SERVER] You must provide a dirt value.")
    return
  end

  veh:setDirt(dirt)

  player:sendChat("[SERVER] Dirt is now " .. dirt .. "!")
end)

RegisterChatCommand("radio", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  if (args[1] == nil) then
    veh:setRadioOn(not veh:getRadioOn())
    player:sendChat("[SERVER] Radio turned to " .. (veh:getRadioOn() and "on" or "off") .. "!")
    return
  end

  local radioStationId = tonumber(args[1])
  if (radioStationId == nil) then
    player:sendChat("[SERVER] Wrong radio id.")
    return
  end

  veh:setRadioStationId(radioStationId)
  veh:setRadioOn(true)

  player:sendChat("[SERVER] Radio station is now " .. veh:getRadioStationId() .. "!")
end)

RegisterChatCommand("rust", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local rust = tonumber(args[1])
  if (rust == nil) then
    player:sendChat("[SERVER] You must provide a rust value.")
    return
  end

  veh:setRust(rust)

  player:sendChat("[SERVER] Rust is now " .. rust .. "!")
end)

RegisterChatCommand("fuel", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local fuel = tonumber(args[1])
  if fuel == nil then
    player:sendChat("[SERVER] You must provide a fuel value.")
    return
  end

  veh:setFuel(fuel)

  player:sendChat("[SERVER] Fuel is now " .. fuel .. "!")
end)

RegisterChatCommand("colors", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local getRandomColor = function ()
    return math.random(0, 255)
  end

  local colorPrimary = RGB.new(getRandomColor(), getRandomColor(), getRandomColor())
  veh:setColorPrimary(colorPrimary)
  player:sendChat("[SERVER] Primary color is now " .. table.concat(colorPrimary:toArray(), ", ") .. "!")

  local colorSecondary = RGB.new(getRandomColor(), getRandomColor(), getRandomColor())
  veh:setColorSecondary(colorSecondary)
  player:sendChat("[SERVER] Secondary color is now " .. table.concat(colorSecondary:toArray(), ", ") .. "!")
end)

RegisterChatCommand("wheelcol", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local getRandomColor = function ()
    return math.random(0, 255)
  end

  local rimColor = RGB.new(getRandomColor(), getRandomColor(), getRandomColor())
  veh:setRimColor(rimColor)
  player:sendChat("[SERVER] Rim color is now " .. table.concat(rimColor:toArray(), ", ") .. "!")

  local tireColor = RGB.new(getRandomColor(), getRandomColor(), getRandomColor())
  veh:setTireColor(tireColor)
  player:sendChat("[SERVER] Tire color is now " .. table.concat(tireColor:toArray(), ", ") .. "!")
end)

RegisterChatCommand("wintint", function (player, message, command, args)
  local veh = player:getVehicle()

  if veh == nil then
    player:sendChat("[SERVER] You're not in a vehicle.")
    return
  end

  local getRandomColor = function ()
    return math.random(0, 255)
  end

  local windowTint = RGBA.new(getRandomColor(), getRandomColor(), getRandomColor(), getRandomColor())
  veh:setWindowTint(windowTint)
  player:sendChat("[SERVER] Window tint is now " .. table.concat(windowTint:toArray(), ", ") .. "!")
end)

RegisterChatCommand("wep", function (player, message, command, args)
  local weaponId = tonumber(args[1]) or 85
  local ammo = tonumber(args[2]) or 200

  player:addWeapon(weaponId, ammo)
  player:sendChat("[SERVER] Weapon " .. weaponId .. " with " .. ammo .. " ammo added!")
end)

RegisterChatCommand("heal", function (player, message, command, args)
  player:setHealth(100.0)
  player:sendChat("[SERVER] You've been healed!")
end)

RegisterChatCommand("tp", function (player, message, command, args)
  local tpDestName = args[1]

  if (tpDestName == nil) then
    player:sendChat("[SERVER] You must provide a teleport destination. (/tp <destination>)")
    player:sendChat("Available destinations: " .. table.concat(utils.getTableKeys(CONFIG.TP_DESTINATIONS), ", "))
    return
  end

  local foundTpDest = CONFIG.TP_DESTINATIONS[tpDestName]

  if (foundTpDest == nil) then
    player:sendChat("[SERVER] Unknown teleport destination \"" .. tpDestName .. "\".")
    return
  end

  player:setPosition(foundTpDest.POSITION)
  player:setRotation(foundTpDest.ROTATION)
  player:sendChat("[SERVER] Teleported to " .. tpDestName .. "!")
end)

RegisterChatCommand("coords", function (player, message, command, args)
  local x = tonumber(args[1])
  local y = tonumber(args[2])
  local z = tonumber(args[3])

  if (x == nil or y == nil or z == nil) then
    player:sendChat("[SERVER] You must provide x, y and z. (/coords <x> <y> <z>)")
    return
  end

  player:setPosition(Vector3.new(x, y, z))
  player:sendChat(string.format("[SERVER] Teleported to %s, %s, %s!", x, y, z))
end)

--- Where am I?
RegisterChatCommand("wai", function (player, message, command, args)
  local pos = player:getPosition()
  local rot = player:getRotation()

  player:sendChat(string.format("[SERVER] Your position: %s, %s, %s", pos.x, pos.y, pos.z))
  player:sendChat(string.format("[SERVER] Your rotation: %s, %s, %s", rot.x, rot.y, rot.z))

  -- Log in console for easy copy-paste
  Console.log(string.format("[GAMEMODE] Player %s position: %s, %s, %s", player.nickname, pos.x, pos.y, pos.z))
  Console.log(string.format("[GAMEMODE] Player %s rotation: %s, %s, %s", player.nickname, rot.x, rot.y, rot.z))
end)

RegisterChatCommand("time", function (player, message, command, args)
  local time = tonumber(args[1])

  if (time == nil or time < 0 or time > 24) then
    player:sendChat("[SERVER] You must provide time between 0.0 to 24.0. (/time <time>)")
    return
  end

  World.setDayTimeHours(time)
end)
