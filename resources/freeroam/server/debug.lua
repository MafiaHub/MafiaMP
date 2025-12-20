local CONFIG = require("freeroam/server/config")

if (CONFIG.ENABLE_DEBUG) then
  Event.onGlobal("onPlayerConnected", function (player)
    Console.log("[DEBUG][onPlayerConnected] player: ", player:toString())
    Console.log("[DEBUG][onPlayerConnected] player id: ", player.id, " | type: ", type(player.id))
    Console.log("[DEBUG][onPlayerConnected] player name: ", player.name, " | type: ", type(player.name))
    Console.log("[DEBUG][onPlayerConnected] player nickname: ", player.nickname, " | type: ", type(player.nickname))
    Console.log("[DEBUG][onPlayerConnected] player modelHash: ",
                player:getModelHash(),
                " | type: ",
                type(player:getModelHash())
    )
    Console.log("[DEBUG][onPlayerConnected] player modelName: ",
                player:getModelName(),
                " | type: ",
                type(player:getModelName())
    )
  end)

  Event.onGlobal("onVehiclePlayerEnter", function (vehicle, player, seatIndex)
    Console.log("[DEBUG][onVehiclePlayerEnter] vehicle: ", vehicle:toString())
    Console.log("[DEBUG][onVehiclePlayerEnter] vehicle id: ", vehicle.id, " | type: ", type(vehicle.id))
    Console.log("[DEBUG][onVehiclePlayerEnter] vehicle name: ", vehicle.name, " | type: ", type(vehicle.name))
    Console.log("[DEBUG][onVehiclePlayerEnter] vehicle nickname: ", vehicle.nickname, " | type: ", type(vehicle.nickname))
    Console.log("[DEBUG][onVehiclePlayerEnter] vehicle player seat index: ", seatIndex, " | type: ", type(seatIndex))
    Console.log("[DEBUG][onVehiclePlayerEnter] player id: ", player.id, " | type: ", type(player.id))
    Console.log("[DEBUG][onPlayerConnected] vehicle modelHash: ",
                vehicle:getModelHash(),
                " | type: ",
                type(vehicle:getModelHash())
    )
    Console.log("[DEBUG][onPlayerConnected] vehicle modelName: ",
                vehicle:getModelName(),
                " | type: ",
                type(vehicle:getModelName())
    )
    Console.log("[DEBUG][onVehiclePlayerEnter] vehicle plate: ",
                vehicle:getLicensePlate(),
                " | type: ",
                type(vehicle:getLicensePlate())
    )
  end)

  Event.onGlobal("onChatCommand", function (player, message, command, args)
    Console.log("[DEBUG][onChatCommand] owner: " .. player.nickname)
    Console.log("[DEBUG][onChatCommand] message: " .. message)
    Console.log("[DEBUG][onChatCommand] command: " .. command)
    Console.log(
      "[DEBUG][onChatCommand] args: " .. table.concat(args, ", "),
      " | type: ",
      type(args),
      " | length: ",
      #args
    )
  end)
end
