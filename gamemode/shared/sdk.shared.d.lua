---@meta _

---@class RGB
---@field r integer # Read-only. Red value (0-255).
---@field g integer # Read-only. Green value (0-255).
---@field b integer # Read-only. Blue value (0-255).
RGB = {}

---Creates a new RGB object.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
---@return RGB
function RGB.new (r, g, b) end

---Converts the color to a string representation.
---<br>(e.g. `RGB{ r: 255, g: 255, b: 255 }`)
---@return string
function RGB:toString () end

---Converts the color to an array of integers {r, g, b}.
---<br>(e.g. `r, g, b = table.unpack(color:toArray())`)
---@return integer[] # An array containing {r, g, b}.
function RGB:toArray () end

---Adds the given RGB values to the color.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
function RGB:add (r, g, b) end

---Subtracts the given RGB values from the color.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
function RGB:sub (r, g, b) end

---Multiplies the color values by the given RGB values.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
function RGB:mul (r, g, b) end

---Divides the color values by the given RGB values.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
function RGB:div (r, g, b) end

---@class RGBA
---@field r integer # Read-only. Red value (0-255).
---@field g integer # Read-only. Green value (0-255).
---@field b integer # Read-only. Blue value (0-255).
---@field a integer # Read-only. Alpha value (0-255).
RGBA = {}

---Creates a new RGBA object.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
---@param a integer # Alpha value (0-255).
---@return RGBA
function RGBA.new (r, g, b, a) end

---Converts the color to a string representation.
---<br>(e.g. `RGBA{ r: 255, g: 255, b: 255, a: 255 }`)
---@return string
function RGBA:toString () end

---Converts the color to an array of integers.
---<br>(e.g. `r, g, b, a = table.unpack(color:toArray())`)
---@return integer[] # An array containing {r, g, b, a}.
function RGBA:toArray () end

---Adds the given RGBA values to the color.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
---@param a integer # Alpha value (0-255).
function RGBA:add (r, g, b, a) end

---Subtracts the given RGBA values from the color.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
---@param a integer # Alpha value (0-255).
function RGBA:sub (r, g, b, a) end

---Multiplies the color values by the given RGBA values.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
---@param a integer # Alpha value (0-255).
function RGBA:mul (r, g, b, a) end

---Divides the color values by the given RGBA values.
---@param r integer # Red value (0-255).
---@param g integer # Green value (0-255).
---@param b integer # Blue value (0-255).
---@param a integer # Alpha value (0-255).
function RGBA:div (r, g, b, a) end

---@type table Console
Console = {}

---Logs a message to the console.
---Accepts a variable number of arguments, all of which are converted to strings and concatenated.
---@param ... any # The arguments to be logged, automatically converted to strings.
---@return string
function Console.log (...) end

---@class Quaternion
---@field w number # Read-only. The w value of the quaternion.
---@field x number # Read-only. The x value of the quaternion.
---@field y number # Read-only. The y value of the quaternion.
---@field z number # Read-only. The z value of the quaternion.
---@field length number # Read-only. The length (magnitude) of the quaternion.
Quaternion = {}

---Creates a new Quaternion.
---@param w number # The w value.
---@param x number # The x value.
---@param y number # The y value.
---@param z number # The z value.
---@return Quaternion
function Quaternion.new (w, x, y, z) end

---Converts the quaternion to a string representation.
---<br>(e.g. `Quaternion{ w: 1.0, x: 1.0, y: 1.0, z: 1.0 }`)
---@return string
function Quaternion:toString () end

---Converts the quaternion to an array of its values.
---<br>(e.g. `w, x, y, z = table.unpack(quat:toArray())`)
---@return number[] # An array containing {w, x, y, z}.
function Quaternion:toArray () end

---Adds another quaternion to this quaternion.
---@param w number # The w value of the quaternion to add.
---@param x number # The x value of the quaternion to add.
---@param y number # The y value of the quaternion to add.
---@param z number # The z value of the quaternion to add.
function Quaternion:add (w, x, y, z) end

---Subtracts another quaternion from this quaternion.
---@param w number # The w value of the quaternion to subtract.
---@param x number # The x value of the quaternion to subtract.
---@param y number # The y value of the quaternion to subtract.
---@param z number # The z value of the quaternion to subtract.
function Quaternion:sub (w, x, y, z) end

---Multiplies this quaternion by another quaternion.
---@param w number # The w value of the quaternion to multiply.
---@param x number # The x value of the quaternion to multiply.
---@param y number # The y value of the quaternion to multiply.
---@param z number # The z value of the quaternion to multiply.
function Quaternion:mul (w, x, y, z) end

---Linearly interpolates between this quaternion and another quaternion.
---@param w number # The w value of the target quaternion.
---@param x number # The x value of the target quaternion.
---@param y number # The y value of the target quaternion.
---@param z number # The z value of the target quaternion.
---@param f number # The interpolation factor (0.0 to 1.0).
function Quaternion:lerp (w, x, y, z, f) end

---Conjugates this quaternion.
---@param w number # The w value of the quaternion to conjugate. *(Note: Typically, conjugation doesn't require additional parameters. If parameters are needed based on your implementation, adjust accordingly.)*
---@param x number # The x value of the quaternion to conjugate.
---@param y number # The y value of the quaternion to conjugate.
---@param z number # The z value of the quaternion to conjugate.
function Quaternion:conjugate (w, x, y, z) end

---Calculates the cross product of this quaternion with another quaternion.
---@param w number # The w value of the quaternion for the cross product.
---@param x number # The x value of the quaternion for the cross product.
---@param y number # The y value of the quaternion for the cross product.
---@param z number # The z value of the quaternion for the cross product.
function Quaternion:cross (w, x, y, z) end

---Calculates the dot product of this quaternion with another quaternion.
---@param w number # The w value of the quaternion for the dot product.
---@param x number # The x value of the quaternion for the dot product.
---@param y number # The y value of the quaternion for the dot product.
---@param z number # The z value of the quaternion for the dot product.
---@return number # The dot product result.
function Quaternion:dot (w, x, y, z) end

---Inverts this quaternion.
function Quaternion:inverse () end

---Sets this quaternion from Euler angles.
---@param x number # The rotation around the x-axis in radians.
---@param y number # The rotation around the y-axis in radians.
---@param z number # The rotation around the z-axis in radians.
function Quaternion:fromEuler (x, y, z) end

---Sets this quaternion from an axis-angle representation.
---@param x number # The x value of the rotation axis.
---@param y number # The y value of the rotation axis.
---@param z number # The z value of the rotation axis.
---@param angle number # The rotation angle in radians.
function Quaternion:fromAxisAngle (x, y, z, angle) end

---@class Vector2
---@field x number # Read-only. The x value of the vector.
---@field y number # Read-only. The y value of the vector.
---@field length number # Read-only. The length (magnitude) of the vector.
Vector2 = {}

---Creates a new Vector2.
---@param x number # The x value.
---@param y number # The y value.
---@return Vector2
function Vector2.new (x, y) end

---Converts the vector to a string representation.
---<br>(e.g. `Vector2{ x: 1.0, y: 1.0 }`)
---@return string
function Vector2:toString () end

---Converts the vector to an array of its values.
---<br>(e.g. `x, y = table.unpack(vec:toArray())`)
---@return number[] # An array containing {x, y}.
function Vector2:toArray () end

---Adds another vector to this vector.
---@param x number # The x value of the vector to add.
---@param y number # The y value of the vector to add.
function Vector2:add (x, y) end

---Subtracts another vector from this vector.
---@param x number # The x value of the vector to subtract.
---@param y number # The y value of the vector to subtract.
function Vector2:sub (x, y) end

---Multiplies this vector by another vector.
---@param x number # The x value of the vector to multiply.
---@param y number # The y value of the vector to multiply.
function Vector2:mul (x, y) end

---Divides this vector by another vector.
---@param x number # The x value of the vector to divide.
---@param y number # The y value of the vector to divide.
function Vector2:div (x, y) end

---Linearly interpolates between this vector and another vector.
---@param x number # The x value of the target vector.
---@param y number # The y value of the target vector.
---@param f number # The interpolation factor (0.0 to 1.0).
function Vector2:lerp (x, y, f) end

---@class Vector3
---@field x number # Read-only. The x value of the vector.
---@field y number # Read-only. The y value of the vector.
---@field z number # Read-only. The z value of the vector.
---@field length number # Read-only. The length (magnitude) of the vector.
Vector3 = {}

---Creates a new Vector3.
---@param x number # The x value.
---@param y number # The y value.
---@param z number # The z value.
---@return Vector3
function Vector3.new (x, y, z) end

---Converts the vector to a string representation.
---<br>(e.g. `Vector3{ x: 1.0, y: 1.0, z: 1.0 }`)
---@return string
function Vector3:toString () end

---Converts the vector to an array of its values.
---<br>(e.g. `x, y, z = table.unpack(vec:toArray())`)
---@return number[] # An array containing {x, y, z}.
function Vector3:toArray () end

---Adds another vector to this vector.
---@param x number # The x value of the vector to add.
---@param y number # The y value of the vector to add.
---@param z number # The z value of the vector to add.
function Vector3:add (x, y, z) end

---Subtracts another vector from this vector.
---@param x number # The x value of the vector to subtract.
---@param y number # The y value of the vector to subtract.
---@param z number # The z value of the vector to subtract.
function Vector3:sub (x, y, z) end

---Multiplies this vector by another vector.
---@param x number # The x value of the vector to multiply.
---@param y number # The y value of the vector to multiply.
---@param z number # The z value of the vector to multiply.
function Vector3:mul (x, y, z) end

---Divides this vector by another vector.
---@param x number # The x value of the vector to divide.
---@param y number # The y value of the vector to divide.
---@param z number # The z value of the vector to divide.
function Vector3:div (x, y, z) end

---Linearly interpolates between this vector and another vector.
---@param x number # The x value of the target vector.
---@param y number # The y value of the target vector.
---@param z number # The z value of the target vector.
---@param f number # The interpolation factor (0.0 to 1.0).
function Vector3:lerp (x, y, z, f) end
