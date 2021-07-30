-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

at = 0
speed_max = 10.0
speed_min = 1.0
speed = 2.0

p = 0

function GenerateLevel()
	--[[
	Generate the entire level 
	
	]]--
	make_box(0.0, 0.0, -36.0, 8.0, 0.5, 36.0, mgRandFloat(), mgRandFloat(), mgRandFloat(), "tile1")
	
	-- Load two tree models
	s_load_model(3.0, 0.0, -4.0, 0.0, 0.0, 0.01, 0.2, "assets://mesh/gum_tree1.obj")
	s_load_model(-3.0, 0.0, -4.0, 0.0, 0.0, -0.05, 0.2, "assets://mesh/gum_tree1.obj")
end

function init()
	-- Set the background colour
	set_background(mgRandFloat(), mgRandFloat(), mgRandFloat(), 1.0)
	
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
	
	print(dt)
	
	-- unrelated
	local x, y, z, rx, ry, rz = get_transform(player)
	
-- 	if at > 1.0 then
		print("Player position: (" .. x .. ", " .. y .. ", " .. z .. ")")
		print("Speed: " .. speed)
		at = 0
-- 	end
	at = at + dt
	
	-- moving the player
	if w then
		speed = speed + dt * 10.0
	end
	
	if s then
		speed = speed - dt * 10.0
	end
	
	-- limit speed to max and min
	speed = math.min(math.max(speed, speed_min), speed_max)
	
	-- move the player
	move_object(player, 0.0, 0.0, -speed * dt)
	
	if a then
		move_object(player, -5.0 * dt, 0.0, 0.0)
	end
	
	if d then
		move_object(player, 5.0 * dt, 0.0, 0.0)
	end
	
	-- jumping
-- 	if j and p < 0 then
-- 		add_force(player, 0.0, 1000.0, 0.0)
-- 		p = 200
-- 	end
	
	p = p - 1
	
	local cx, cy, cz, crx, cry, crz = get_transform(cam)
	
	cx = x * 0.2
	cy = y + 4.0
	cz = z + 8.0
	
	push_transform(cam, cx, cy, cz, crx, cry, crz)
end

function free()
end
