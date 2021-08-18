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

function init()
	-- Create camera
	cam = create_entity(ENT_TRANSFORM)
	push_transform(cam, 0.0, 5.0, 5.65, 0.0375, 0.0, 0.0)
	set_camera(cam)
	
	-- Initialise player
	player = make_box(0.0, 2.0, -2.0)
	set_physics_flags(player, PHYSICS_MODE_PLAYER)
	
	-- Sync the physics graph after everything has been created
	physics_sync_graph()
	
	-- Set some registry values
	reg_set("life", "3")
	reg_set("offset", "5230")
	
	-- Open the level script
	level = script_open("assets://levels/test.lua")
	
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
		print("LIFE: " .. reg_get("life"), "OFFSET: " .. reg_get("offset"))
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
		add_force(player, 0.0, 1250.0, 0.0)
		p = 200
	end
	
	p = p - 1
	
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

function free()
	script_destroy(level)
end
