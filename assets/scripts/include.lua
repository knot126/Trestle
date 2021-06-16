-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Include Script
--
-- * This script is included before most every script is run
-- * This should contain only helper functions in lua
-- 

QR_COMPONENT_TRANSFORM = (1 << 0)
QR_COMPONENT_MESH = (1 << 1)
QR_COMPONENT_CAMERA = (1 << 2)
QR_COMPONENT_PHYSICS = (1 << 3)
QR_COMPONENT_AABB = (1 << 4)

QR_ELEMUI_BOX = (1 << 0)
QR_ELEMUI_TEXT = (1 << 1)

QR_PHYS_DISABLE_GRAVITY = (1 << 0)
QR_PHYS_ENABLE_RESPONSE = (1 << 2)

function new_cube(x, y, z, sx, sy, sz, phys, pflags, model)
	local flags = QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH
	if phys then
		flags = flags | QR_COMPONENT_PHYSICS
	end
	local ent = mgEntity(flags)
	
	mgTransform(ent, x, y, z, 0.0, 0.0, 0.0, sx, sy, sz)
	mgMesh2(ent, model)
	if phys then
		mgMass(ent, 1.0)
		mgForce(ent, 0.0, 0.0, -100.0, 0.0, 0.0, 0.0)
		if pflags ~= nil then
			mgPhysFlags(ent, pflags)
		end
	end
	
	return ent
end

function buildWallsAndFloor(w, h, l, offset)
	if offset == nil then offset = 0.0 end
	
	-- walls and floor
	-- new_cube(0.0, -0.75, -l / 2.0 - offset, w, 0.5, l, false, nil, "assets://mesh/cube4.xml")
	new_cube(-w, (h / 2.0) - 0.75, -l / 2.0 - offset, 0.5, h, l, false, nil, "assets://mesh/cube4.xml")
	new_cube(w, (h / 2.0) - 0.75, -l / 2.0 - offset, 0.5, h, l, false, nil, "assets://mesh/cube4.xml")
	
	-- decor
	for z = 1.0, tonumber(l), 2.0 do
		new_cube(-w + 0.5, (h / 2.0) - 0.75, -z - offset, 0.5, h, 0.5, false, nil, "assets://mesh/cube4.xml")
		new_cube(-w + 1.0, 0.25, -z - offset, 0.5, 0.5, 0.5, false, nil, "assets://mesh/cube4.xml")
		
		new_cube(w - 0.5, (h / 2.0) - 0.75, -z - offset, 0.5, h, 0.5, false, nil, "assets://mesh/cube4.xml")
		new_cube(w - 1.0, 0.25, -z - offset, 0.5, 0.5, 0.5, false, nil, "assets://mesh/cube4.xml")
	end
end

function buildFloorTest(w, h, l, offset)
	if offset == nil then offset = 0.0 end
	
	local last = offset
	
	while last <= l + offset do
		local length = (mgRandFloat() - 0.5) * w + w
		--new_cube(mgRandFloat() * w, -0.75, -last - length / 2.0, length, 0.5, length, false, nil, "assets://mesh/cube4.xml")
		new_cube(((mgRandFloat() * w) - (w / 2.0)) * 2.0, mgRandFloat() - 0.5, -last - length / 2.0, 1.5, 0.5, 1.5, false, nil, "assets://mesh/cube4.xml")
		last = last + length
	end
end

function createUIText(text, x, y, size)
	local e = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(e, text)
	mgUITextPos(e, x, y)
	mgUITextSize(e, size)
	
	return e
end
