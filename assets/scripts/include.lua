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

-- glfw keys
GLFW_KEY_ENTER = 257
GLFW_KEY_RIGHT = 262
GLFW_KEY_LEFT = 263
GLFW_KEY_DOWN = 264
GLFW_KEY_UP = 265

function new_cube(x, y, z, sx, sy, sz, phys, pflags, model)
	local flags = QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH
	if phys then
		flags = flags | QR_COMPONENT_PHYSICS
	end
	local ent = mgEntity(flags)
	
	mgTransform(ent, x, y, z, 0.0, 0.0, 0.0, sx, sy, sz)
	if string.find(model, ".obj") then
		mgObjMesh(ent, model)
	else
		mgMesh2(ent, model)
	end
	if phys then
		mgMass(ent, 1.0)
		mgForce(ent, 0.0, 0.0, -100.0, 0.0, 0.0, 0.0)
		if pflags ~= nil then
			mgPhysFlags(ent, pflags)
		end
	end
	
	return ent
end
--[[
function buildWallsAndFloor(w, h, l, offset)
	if offset == nil then offset = 0.0 end
	
	-- walls and floor
	-- new_cube(0.0, -0.75, -l / 2.0 - offset, w, 0.5, l, false, nil, "assets://mesh/cube4.xml")
	mgBox(-w, (h / 2.0) - 0.75, -l / 2.0 - offset, 0.5, h, l)
	mgBox(w, (h / 2.0) - 0.75, -l / 2.0 - offset, 0.5, h, l)
	
	-- decor
	for z = 1.0, tonumber(l), 2.0 do
		mgBox(-w + 0.5, (h / 2.0) - 0.75, -z - offset, 0.5, h, 0.5)
		mgBox(-w + 1.0, 0.25, -z - offset, 0.5, 0.5, 0.5)
		
		mgBox(w - 0.5, (h / 2.0) - 0.75, -z - offset, 0.5, h, 0.5)
		mgBox(w - 1.0, 0.25, -z - offset, 0.5, 0.5, 0.5)
	end
end

function buildFloorTest2(w, h, l, offset)
	if offset == nil then offset = 0.0 end
	
	local last = offset
	
	while last <= l + offset do
		local length = (mgRandFloat() - 0.5) * w + w
		mgBox(
			math.floor(((mgRandFloat() * w) - (w / 2.0)) * 2.0), 
			math.floor(mgRandFloat() - 0.5), 
			math.floor(-last - length / 2.0), 
			math.floor(mgRandFloat() + 1.5), 
			math.floor(mgRandFloat() + 1.5) * 0.2, 
			math.floor(mgRandFloat() + 1.5), 
			mgRandFloat(), 
			mgRandFloat(), 
			mgRandFloat(),
			"tile1")
		last = last + length
	end
end

function buildFloorTest(w, h, l, offset)
	if offset == nil then offset = 0.0 end
	
	local last = offset
	
	while last <= l + offset do
		local length = (mgRandFloat() - 0.5) * w + w
		mgBox(
			math.floor(((mgRandFloat() * w) - (w / 2.0)) * 2.0), 
			math.floor(mgRandFloat() - 0.5), 
			math.floor(-last - length / 2.0), 
			math.floor(mgRandFloat() + 1.5), 
			math.floor(mgRandFloat() + 1.5) * 0.2, 
			math.floor(mgRandFloat() + 1.5), 
			mgRandFloat(), 
			mgRandFloat(), 
			mgRandFloat(),
			"tile1")
		last = last + length + 1.0
	end
end 

function createUIText(text, x, y, size)
	local e = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(e, text)
	mgUITextPos(e, x, y)
	mgUITextSize(e, size)
	
	return e
end
]]--

-- prototyping menu

function createMenu(options, x)
	local m = {}
	
	for i = 0, #options do
		m["T" .. tostring(i)] = createUIText(options[i], x, 1.0 - (i * 0.12), 0.1)
	end
	
	m.current = 0
	
	return m
end

function updateMenu(m)
	mgUITextColour(m["T" .. tostring(m.current)], 0.5, 0.5, 1.0, 1.0)
	
	return m
end

-- CrabUI Library
crab = {
	options = {},
	active = 1,
	updated = true,
	kp = 0,
	
	addOption = function(this, x, y, text, values)
		i = #this.options + 1
		this.options[i] = {}
		id = mgUIElement(QR_ELEMUI_TEXT)
		mgUIText(id, text)
		mgUITextPos(id, x, y)
		mgUITextSize(id, 0.1)
		mgUITextColour(id, 1.0, 1.0, 1.0, 1.0)
		
		this.options[i].id = id
		this.options[i].text = text
		this.options[i].values = values
		this.options[i].active = 1
		
		return i
	end,
	
	getValue = function(this, index)
		return this.options[index].values[this.options[this.active].active]
	end,
	
	update = function(this)
		local up = mgGetKey(265)
		local down = mgGetKey(264)
		local left = mgGetKey(263)
		local right = mgGetKey(262)
		
		if up and this.kp <= 0 then
			this.active = this.active - 1
			this.updated = true
			this.kp = 30
		end
		
		if down and this.kp <= 0 then
			this.active = this.active + 1
			this.updated = true
			this.kp = 30
		end
		
		if left and this.kp <= 0 then
			this.options[this.active].active = this.options[this.active].active - 1
			this.updated = true
			this.kp = 30
		end
		
		if right and this.kp <= 0 then
			this.options[this.active].active = this.options[this.active].active + 1
			this.updated = true
			this.kp = 30
		end
		
		if this.options[this.active].active < 1 then this.options[this.active].active = 1 end
		if this.options[this.active].active > #this.options[this.active].values then this.options[this.active].active = #this.options[this.active].values end
		
		this.kp = this.kp - 1
		
		if this.active < 1 then this.active = 1 end
		if this.active > #this.options then this.active = #this.options end
		
		if this.updated then
			for i = 1, #this.options, 1 do
				if this.active == i then
					mgUITextColour(this.options[i].id, 0.0, 1.0, 0.0, 1.0)
				else
					mgUITextColour(this.options[i].id, 1.0, 1.0, 1.0, 1.0)
				end
				mgUIText(this.options[i].id, this.options[i].text .. ": " .. this.options[i].values[this.options[i].active])
			end
		end
		
		this.updated = false
	end
}
