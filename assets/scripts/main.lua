-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

at = 0
SPEED_MAX = 15.0
SPEED_MIN = 3.0
speed = 6.0

p = 0
toruses = {}

-- r, g, b = mgRandFloat(), mgRandFloat(), mgRandFloat()
r, g, b = 1.0, 1.0, 1.0

function RandPosAxis(max, diss)
	a = (mgRandFloat() - 0.5) * 2.0 * max
	if a > diss or a < -diss then
		return a
	elseif a > 0.0 then
		return diss
	else
		return -diss
	end
end

function RandNegate(n)
	if mgRandInt() % 2 == 1 then
		return n
	else
		return -n
	end
end

function GenerateLevel()
	--[[
	Generate the entire level 
	
	]]--
	make_box(0.0, 0.0, -36.0, 8.0, 0.5, 36.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	
	-- Load some tree models for the forest
	for z = 0.0, 150.0, 12.0 do
		s_load_model(RandPosAxis(16.0, 8.0), 0.0, -z, 0.0, 0.0, 0.0, 0.2, "assets://mesh/gum_tree1.obj")
	end
	
	toruses[1] = s_load_model(2.5, 1.0, -48.0, mgRandFloat(), mgRandFloat(), mgRandFloat(), 2.0, "assets://mesh/torus.obj")
	toruses[2] = s_load_model(-2.5, 1.0, -32.0, mgRandFloat(), mgRandFloat(), mgRandFloat(), 2.0, "assets://mesh/torus.obj")
	
	for l = 72.0, 250.0, 1.0 do
		make_box(math.sin(l) * 8.0, math.cos(l) * 8.0, -l, 2.0, 1.0, 2.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	end
	
	for l = 0.0, 72.0, 12.0 do
		make_box(RandNegate(5.0), 0.0, -l, 1.0, 16.0, 1.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	end
end

function UpdateRotation(e, dt)
	local x, y, z, rx, ry, rz, sx, sy, sz = get_transform(e)
	
	rx = rx + dt
	ry = ry + dt
	rz = rz + dt
	
	push_transform(e, x, y, z, rx, ry, rz, sx, sy, sz)
end

function init()
	-- Set the background colour
	set_background(r, g, b, 1.0)
	
	-- Create camera
	cam = create_entity(ENT_TRANSFORM)
	push_transform(cam, 0.0, 5.0, 5.65, 0.0375, 0.0, 0.0)
	set_camera(cam)
	
	-- Initialise player
	player = make_box(0.0, 2.0, -2.0)
	set_physics_flags(player, PHYSICS_MODE_PLAYER)
	
	-- Generate the level
	GenerateLevel()
	
	-- Sync the physics graph after everything has been created
	physics_sync_graph()
	
	-- Enable the physics engine
	enable_physics(true)
end

function tick(dt)
	local w = get_key(GLFW_KEY_UP)
	local s = get_key(GLFW_KEY_DOWN)
	local a = get_key(GLFW_KEY_LEFT)
	local d = get_key(GLFW_KEY_RIGHT)
	local j = get_key(GLFW_KEY_SPACE)
	
	-- unrelated
	local x, y, z, rx, ry, rz = get_transform(player)
	
	if at > 1.0 then
		print("POSITION (" .. x .. ", " .. y .. ", " .. z .. ")", "SPEED " .. speed)
		at = 0
	end
	at = at + dt
	
	-- moving the player
	if w then
		speed = speed + dt * 5.0
	end
	
	if s then
		speed = speed - dt * 5.0
	end
	
	-- limit speed to max and min
	speed = math.min(math.max(speed, SPEED_MIN), SPEED_MAX)
	
	-- move the player
	move_object(player, 0.0, 0.0, -speed * dt)
	
	if a then
		move_object(player, -5.0 * dt, 0.0, 0.0)
	end
	
	if d then
		move_object(player, 5.0 * dt, 0.0, 0.0)
	end
	
	-- jumping
	if j and p < 0 then
		add_force(player, 0.0, 1000.0, 0.0)
		p = 200
	end
	
	p = p - 1
	
	-- update the rotations
	UpdateRotation(toruses[1], dt)
	UpdateRotation(toruses[2], dt)
	
	-- Update the camera's position
	local cx, cy, cz, crx, cry, crz = get_transform(cam)
	
	cx = x * 0.2
	cy = y + 4.0
	cz = z + 8.0
	
	push_transform(cam, cx, cy, cz, crx, cry, crz)
	
	-- Check for player's death
	if y < -8.0 then
		quit()
	end
end
