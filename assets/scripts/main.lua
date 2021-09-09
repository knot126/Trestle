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
	
	-- Try making a rectangle
	R = make_rect(0.0, 0.875, 2.0, 0.25, 0.0, 0.0, 0.0, 0.4)
	
	-- Open the level script
	level = script_open("assets://levels/test.lua")
	
	-- Enable the physics engine
	enable_physics(true)
	
	-- Disable mouse cursor (for camera)
	set_mouse_disabled(true)
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
		print("Position: (" .. x .. ", " .. y .. ", " .. z .. ")")
		at = 0
	end
	at = at + dt
	
	-- limit speed to max and min
	speed = math.min(math.max(speed, SPEED_MIN), SPEED_MAX)
	
	fx, fy, fz = get_camera_forward(0.0, 0.0, -1.0)
	
	print(fx,fy,fz)
	
	if w then
		move_object(player, fx * -10.0 * dt, 0.0, fz * 10.0 * dt)
	end
	
	if s then
		move_object(player, 0.0, 0.0, 10.0 * dt)
	end
	
-- 	if a then
-- 		move_object(player, -5.0 * dt, 0.0, 0.0)
-- 	end
-- 	
-- 	if d then
-- 		move_object(player, 5.0 * dt, 0.0, 0.0)
-- 	end
	
	-- jumping
	if j and p < 0 then
		add_force(player, 0.0, 1250.0, 0.0)
		p = 200
	end
	
	-- Update the camera's position
	local sx, sy = get_screen_size()
	local mx, my = get_mouse_pos()
	mx, my = ((mx / sx) - 0.5) * 0.3, ((my / sy) - 0.5) * 0.3
	local cx, cy, cz, crx, cry, crz = get_transform(cam)
	
	cx = x
	cy = y + 3.0
	cz = z
	crx = my
	cry = mx
	
	push_transform(cam, cx, cy, cz, crx, cry, crz)
end

function free()
	script_destroy(level)
end
