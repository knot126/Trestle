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

cubes = {}

function new_cube(x, y, z, sx, sy, sz, phys)
	flags = QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH
	if phys then
		flags = flags | QR_COMPONENT_PHYSICS
	end
	ent = mgCreateEntity(flags)
	
	mgTransform(ent, x, y, z, 0.0, 0.0, 0.0, sx, sy, sz)
	mgMesh(ent, "assets://mesh/cube2.bin")
	if phys then
		mgForce(ent, 0.0, 0.0, -100.0, 0.0, 0.0, 0.0)
		mgMass(ent, 1.0)
	end
	
	return ent
end

for z = 0.0, 16.0, 1.0 do
	cubes[#cubes + 1] = new_cube(
		(mgRandFloat() - 0.5) * 5.0, (mgRandFloat() - 0.5) * 5.0, -z,
		mgRandFloat(), mgRandFloat(), mgRandFloat(),
		true)
	print("Created cube with entity ID ", ent)
end
