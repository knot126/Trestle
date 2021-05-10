-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

QR_COMPONENT_TRANSFORM = (1 << 0)
QR_COMPONENT_MESH = (1 << 1)
QR_COMPONENT_CAMERA = (1 << 2)
QR_COMPONENT_PHYSICS = (1 << 3)
QR_COMPONENT_AABB = (1 << 4)

QR_PHYS_DISABLE_GRAVITY = (1 << 0)
QR_PHYS_ENABLE_RESPONSE = (1 << 2)

cubes = {}

function new_cube(x, y, z, sx, sy, sz, phys, pflags)
	flags = QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH
	if phys then
		flags = flags | QR_COMPONENT_PHYSICS
	end
	ent = mgCreateEntity(flags)
	
	mgTransform(ent, x, y, z, 0.0, 0.0, 0.0, sx, sy, sz)
	mgMesh(ent, "assets://mesh/cube2.bin")
	if phys then
		mgMass(ent, 1.0)
		mgForce(ent, 0.0, 0.0, -100.0, 0.0, 0.0, 0.0)
		if pflags ~= nil then
			mgPhysFlags(ent, pflags)
		end
	end
	
	return ent
end

cam = mgCamera()

--[[
for z = 0.0, 16.0, 1.0 do
	cubes[#cubes + 1] = new_cube(
		(mgRandFloat() - 0.5) * 5.0, (mgRandFloat() - 0.5) * 5.0, -z,
		mgRandFloat(), mgRandFloat(), mgRandFloat(),
		true, nil)
	--print("Crated cube with entity ID ", ent)
end
]]--

--new_cube(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, false)
a = new_cube(0.0, 0.0, -16.0, 5.0, 0.5, 32.0, false, nil)
b = new_cube(-2.5, 2.0, 0.0, 0.5, 4.0, 3.0, false, nil)
c = new_cube(2.5, 2.0, 0.0, 0.5, 4.0, 3.0, false, nil)

-- player init
player = new_cube(0.0, 1.5, 0.0, 1.0, 1.0, 1.0, true, QR_PHYS_ENABLE_RESPONSE)
mgActivePlayer(player)
print("Player is ", player, " cube are ", a, b, c)
