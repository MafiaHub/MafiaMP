---@meta _

---@class Entity
---@field id number # Read-only. The entity's ID.
---@field name string # Read-only. The entity's name.
---@field nickname string # Read-only. The entity's nickname.
Entity = {}

---Destroys the entity.
function Entity:destroy () end

---Checks if the entity is always visible.
---@return boolean
function Entity:getAlwaysVisible () end

---Sets the entity's always-visible state.
---@param visible boolean
function Entity:setAlwaysVisible (visible) end

---Gets the entity's model hash.
---@return number
function Entity:getModelHash () end

---Sets the entity's model hash.
---@param hash number
function Entity:setModelHash (hash) end

---Gets the entity's model name.
---@return string
function Entity:getModelName () end

---Sets the entity's model name.
---@param name string
function Entity:setModelName (name) end

---Gets the entity's position.
---@return Vector3
function Entity:getPosition () end

---Sets the entity's position.
---@param position Vector3
function Entity:setPosition (position) end

---Gets the entity's rotation.
---@return Quaternion
function Entity:getRotation () end

---Sets the entity's rotation.
---@param rotation Quaternion
function Entity:setRotation (rotation) end

---Gets the entity's scale.
---@return Vector3
function Entity:getScale () end

---Sets the entity's scale.
---@param scale Vector3
function Entity:setScale (scale) end

---Gets the entity's update interval.
---@return number
function Entity:getUpdateInterval () end

---Sets the entity's update interval.
---@param interval number
function Entity:setUpdateInterval (interval) end

---Gets the entity's velocity.
---@return Vector3
function Entity:getVelocity () end

---Sets the entity's velocity.
---@param velocity Vector3
function Entity:setVelocity (velocity) end

---Gets the entity's virtual world ID.
---@return number
function Entity:getVirtualWorld () end

---Sets the entity's virtual world ID.
---@param virtualWorld number
function Entity:setVirtualWorld (virtualWorld) end

---Checks if the entity is visible.
---@return boolean
function Entity:getVisible () end

---Sets the entity's visibility.
---@param visible boolean
function Entity:setVisible (visible) end

---Converts the entity to a string representation.
---<br>(e.g. `Entity{ id: 1 }`)
---@return string
function Entity:toString () end

---@class Human: Entity
Human = {}

---Adds a weapon to the Human.
---@param weaponId integer # The weapon ID to add.
---@param ammo integer # The amount of ammo for the weapon.
function Human:addWeapon (weaponId, ammo) end

---Gets current aim direction
function Human:getAimDir () end

---Gets current aim hit position
function Human:getAimPos () end

---Gets current weaponId
function Human:getWeapon () end

---Gets whether human is aiming
function Human:isAiming () end

---Gets whether human is firing
function Human:isFiring () end

---Sets the Human's health.
---@param health number # The health value to set (0.0 to 100.0).
function Human:setHealth (health) end

---Gets the Human's current health.
---@return number # The current health value (0.0 to 100.0).
function Human:getHealth () end

---Retrieves the vehicle the Human is in.
---@return Vehicle # The vehicle the Human is in.
function Human:getVehicle () end

---Retrieves the seat index the Human occupies in the vehicle.
---@return integer # The seat index the Human occupies. Returns -1 if the Human is not in a vehicle.
function Human:getVehicleSeatIndex () end

---@class Player: Human
Player = {}

---Destroys the Player object
---@private # Player object can not be destroyed
function Player:destroy () end

---Sends a chat message from the player to another player.
---@param message string # The chat message.
function Player:sendChat (message) end

---Sends a chat message from the player to all players.
---@param message string # The chat message.
function Player:sendChatToAll (message) end

---@class Vehicle: Entity
Vehicle = {}

---Checks if beacon lights are on.
---@return boolean
function Vehicle:getBeaconLightsOn () end

---Sets the beacon lights state.
---@param on boolean
function Vehicle:setBeaconLightsOn (on) end

---Gets the primary color of the vehicle.
---@return RGB
function Vehicle:getColorPrimary () end

---Sets the primary color of the vehicle.
---@param color RGB
function Vehicle:setColorPrimary (color) end

---Gets the secondary color of the vehicle.
---@return RGB
function Vehicle:getColorSecondary () end

---Sets the secondary color of the vehicle.
---@param color RGB
function Vehicle:setColorSecondary (color) end

---Gets the vehicle's dirt level.
---@return number
function Vehicle:getDirt () end

---Sets the vehicle's dirt level.
---@param dirt number
function Vehicle:setDirt (dirt) end

---Checks if the engine is on.
---@return boolean
function Vehicle:getEngineOn () end

---Sets the engine state.
---@param on boolean
function Vehicle:setEngineOn (on) end

---Gets the fuel level of the vehicle.
---@return number
function Vehicle:getFuel () end

---Sets the fuel level of the vehicle.
---@param fuel number
function Vehicle:setFuel (fuel) end

---Gets the vehicle's license plate.
---@return string
function Vehicle:getLicensePlate () end

---Sets the vehicle's license plate.
---@param plate string
function Vehicle:setLicensePlate (plate) end

---Gets the vehicle's lock state.
---@return integer
function Vehicle:getLockState () end

---Sets the vehicle's lock state.
---@param lockState integer
function Vehicle:setLockState (lockState) end

---Checks if the radio is on.
---@return boolean
function Vehicle:getRadioOn () end

---Sets the radio state.
---@param on boolean
function Vehicle:setRadioOn (on) end

---Gets the current radio station ID.
---@return number
function Vehicle:getRadioStationId () end

---Sets the radio station ID.
---@param id number
function Vehicle:setRadioStationId (id) end

---Gets the vehicle's rim color.
---@return RGB
function Vehicle:getRimColor () end

---Sets the vehicle's rim color.
---@param color RGB
function Vehicle:setRimColor (color) end

---Gets the vehicle's rust level.
---@return number
function Vehicle:getRust () end

---Sets the vehicle's rust level.
---@param rust number
function Vehicle:setRust (rust) end

---Checks if the siren is on.
---@return boolean
function Vehicle:getSirenOn () end

---Sets the siren state.
---@param on boolean
function Vehicle:setSirenOn (on) end

---Gets the vehicle's tire color.
---@return RGB
function Vehicle:getTireColor () end

---Sets the vehicle's tire color.
---@param color RGB
function Vehicle:setTireColor (color) end

---Gets the window tint color of the vehicle.
---@return RGBA
function Vehicle:getWindowTint () end

---Sets the window tint color of the vehicle.
---@param color RGBA
function Vehicle:setWindowTint (color) end

---@type table Chat
Chat = {}

---Sends a chat message to a specific player.
---@param player Player # The player to send the message to.
---@param message string # The message to send.
function Chat.sendToPlayer (player, message) end

---Sends a chat message to all players.
---@param message string # The message to send.
function Chat.sendToAll (message) end

---@type table World
World = {}

---Sets the weather based on the given weather set name.
---@param weatherSetName string # The name of the weather set to apply.
function World.setWeather (weatherSetName) end

---Sets the daytime hours (0.0 to 24.0).
---@param dayTimeHours number # The time of day in hours (0.0-24.0).
function World.setDayTimeHours (dayTimeHours) end

---Creates a new vehicle with the given model name.
---@param modelName string # The model name of the vehicle.
---@return Vehicle # The created vehicle object.
function World.createVehicle (modelName) end

---Callback definition for onGamemodeLoaded event.
---@alias OnGamemodeLoadedCallback fun()

--- Registers a callback to be invoked when the gamemode is loaded.
---@param eventName "onGamemodeLoaded" # The name of the event.
---@param eventCallback OnGamemodeLoadedCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onGamemodeUnloading event.
---@alias OnGamemodeUnloadingCallback fun()

--- Registers a callback to be invoked before the gamemode is unloaded.
---@param eventName "onGamemodeUnloading" # The name of the event.
---@param eventCallback OnGamemodeUnloadingCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onGamemodeUpdated event.
---@alias OnGamemodeUpdatedCallback fun()

---Registers a callback to be invoked when the gamemode is updated.
---WARNING: This event is called every frame, so be cautious with the code you put here.
---@param eventName "onGamemodeUpdated" # The name of the event.
---@param eventCallback OnGamemodeUpdatedCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onPlayerConnected event.
---@alias OnPlayerConnectedCallback fun(player: Player)

---Registers a callback to be invoked when a player connects to the server.
---@param eventName "onPlayerConnected" # The name of the event.
---@param eventCallback OnPlayerConnectedCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onPlayerDisconnected event.
---@alias OnPlayerDisconnectedCallback fun(player: Player)

---Registers a callback to be invoked when a player disconnects from the server.
---@param eventName "onPlayerDisconnected" # The name of the event.
---@param eventCallback OnPlayerDisconnectedCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onPlayerDied event.
---@alias OnPlayerDiedCallback fun(player: Player)

---Registers a callback to be invoked when a player dies.
---@param eventName "onPlayerDied" # The name of the event.
---@param eventCallback OnPlayerDiedCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onChatMessage event.
---@alias OnChatMessageCallback fun(player: Player, message: string)

---Registers a callback to be invoked when a player sends a chat message.
---@param eventName "onChatMessage" # The name of the event.
---@param eventCallback OnChatMessageCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onChatCommand event.
---@alias OnChatCommandCallback fun(player: Player, message: string, command: string, args: string[])

---Registers a callback to be invoked when a player issues a chat command.
---@param eventName "onChatCommand" # The name of the event.
---@param eventCallback OnChatCommandCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onVehiclePlayerEnter event.
---@alias OnVehiclePlayerEnterCallback fun(vehicle: Vehicle, player: Player, seatIndex: integer)

---Registers a callback to be invoked when a player enters a vehicle.
---@param eventName "onVehiclePlayerEnter" # The name of the event.
---@param eventCallback OnVehiclePlayerEnterCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for onVehiclePlayerLeave event.
---@alias OnVehiclePlayerLeaveCallback fun(vehicle: Vehicle, player: Player)

---Registers a callback to be invoked when a player leaves a vehicle.
---@param eventName "onVehiclePlayerLeave" # The name of the event.
---@param eventCallback OnVehiclePlayerLeaveCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end

---Callback definition for custom events.
---@alias OnCustomEventCallback fun(...: any)

---Registers a callback for a custom event.
---@param eventName string # The name of the custom event.
---@param eventCallback OnCustomEventCallback # The function to execute when the event occurs.
function listenEvent (eventName, eventCallback) end
