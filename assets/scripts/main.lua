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

freecam = true
camspeed = 10.0

-- function mkp(b, uw, vw, a)
-- 	push_patch(b, 3, 3,
--        0.0, mgRandFloat() * a, 0.0,  0.0, mgRandFloat() * a, 0.5 * vw,  0.0, mgRandFloat() * a, 1.0 * vw,
--        0.5 * uw, mgRandFloat() * a, 0.0,  0.5 * uw, mgRandFloat() * a, 0.5 * vw,  0.5 * uw, mgRandFloat() * a, 1.0 * vw,
--        1.0 * uw, mgRandFloat() * a, 0.0,  1.0 * uw, mgRandFloat() * a, 0.5 * vw,  1.0 * uw, mgRandFloat() * a, 1.0 * vw
-- 	)
-- end

function mkp(b, uw, vw, a) push_patch(b, 6, 6,  0.0 * uw, mgRandFloat() * a, 0.0 * vw,  0.0 * uw, mgRandFloat() * a, 0.2 * vw,  0.0 * uw, mgRandFloat() * a, 0.4 * vw,  0.0 * uw, mgRandFloat() * a, 0.6 * vw,  0.0 * uw, mgRandFloat() * a, 0.8 * vw,  0.0 * uw, mgRandFloat() * a, 1.0 * vw, 
 0.2 * uw, mgRandFloat() * a, 0.0 * vw,  0.2 * uw, mgRandFloat() * a, 0.2 * vw,  0.2 * uw, mgRandFloat() * a, 0.4 * vw,  0.2 * uw, mgRandFloat() * a, 0.6 * vw,  0.2 * uw, mgRandFloat() * a, 0.8 * vw,  0.2 * uw, mgRandFloat() * a, 1.0 * vw, 
 0.4 * uw, mgRandFloat() * a, 0.0 * vw,  0.4 * uw, mgRandFloat() * a, 0.2 * vw,  0.4 * uw, mgRandFloat() * a, 0.4 * vw,  0.4 * uw, mgRandFloat() * a, 0.6 * vw,  0.4 * uw, mgRandFloat() * a, 0.8 * vw,  0.4 * uw, mgRandFloat() * a, 1.0 * vw, 
 0.6 * uw, mgRandFloat() * a, 0.0 * vw,  0.6 * uw, mgRandFloat() * a, 0.2 * vw,  0.6 * uw, mgRandFloat() * a, 0.4 * vw,  0.6 * uw, mgRandFloat() * a, 0.6 * vw,  0.6 * uw, mgRandFloat() * a, 0.8 * vw,  0.6 * uw, mgRandFloat() * a, 1.0 * vw, 
 0.8 * uw, mgRandFloat() * a, 0.0 * vw,  0.8 * uw, mgRandFloat() * a, 0.2 * vw,  0.8 * uw, mgRandFloat() * a, 0.4 * vw,  0.8 * uw, mgRandFloat() * a, 0.6 * vw,  0.8 * uw, mgRandFloat() * a, 0.8 * vw,  0.8 * uw, mgRandFloat() * a, 1.0 * vw, 
 1.0 * uw, mgRandFloat() * a, 0.0 * vw,  1.0 * uw, mgRandFloat() * a, 0.2 * vw,  1.0 * uw, mgRandFloat() * a, 0.4 * vw,  1.0 * uw, mgRandFloat() * a, 0.6 * vw,  1.0 * uw, mgRandFloat() * a, 0.8 * vw,  1.0 * uw, mgRandFloat() * a, 1.0 * vw
) end


function init()
	-- Create camera
	cam = create_entity(ENT_TRANSFORM)
	push_transform(cam, 0.0, 5.0, 5.65, 0.0375, 0.0, 0.0)
	set_camera(cam)
	
	-- Initialise player
	player = make_box(0.0, 2.0, -2.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, "tile0")
	set_physics_flags(player, PHYSICS_MODE_PLAYER)
	
	-- Some bezier patch
	be = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_SURFACE)
	mkp(be, 5.0, 5.0, 5.0)
-- 	mkp(be, 5.0, 5.0, 5.0)
	
	push_transform(be, 0.0, 0.0, 0.0)
	
	-- Sync the physics graph after everything has been created
	physics_sync_graph()
	
	-- Set some registry values
	reg_set("life", "3")
	reg_set("offset", "5230")
	
	-- Open the level script
	level = script_open("assets://levels/pcgroom.lua")
	
	-- Enable the physics engine
	enable_physics(true)
	
	-- Disable mouse cursor (for camera)
	set_mouse_disabled(true)
	
	-- Add actions for keys
	RegisterAction("up", {GLFW_KEY_W, GLFW_KEY_UP})
	RegisterAction("down", {GLFW_KEY_S, GLFW_KEY_DOWN})
	RegisterAction("left", {GLFW_KEY_A, GLFW_KEY_LEFT})
	RegisterAction("right", {GLFW_KEY_D, GLFW_KEY_RIGHT})
	RegisterAction("jump", {GLFW_KEY_SPACE})
end

function tick(dt)
	local w = GetAction("up")
	local s = GetAction("down")
	local a = GetAction("left")
	local d = GetAction("right")
	local j = GetAction("jump")
	
	-- unrelated
	local x, y, z, rx, ry, rz = get_transform(be)
	
	if at > 1.0 then
		print("Position: (" .. x .. ", " .. y .. ", " .. z .. ")")
		at = 0
	end
	at = at + dt
	
	-- limit speed to max and min
	speed = math.min(math.max(speed, SPEED_MIN), SPEED_MAX)
	
	fx, fy, fz = get_camera_forward(0.0, 0.0, -1.0)
	
-- 	if w then
-- 		move_object(player, fx * (-10.0 * dt), 0.0, fz * (10.0 * dt))
-- 	end
-- 	
-- 	if s then
-- 		move_object(player, fx * (10.0 * dt), 0.0, fz * (-10.0 * dt))
-- 	end
-- 	
-- 	if j then
-- 		add_force(player, 0.0, 100.0, 0.0)
-- 	end
	
	-- Update the camera's position
	local sx, sy = get_screen_size()
	local mx, my = get_mouse_pos()
	mx, my = ((mx / sx) - 0.5) * 0.3, ((my / sy) - 0.5) * 0.3
	local cx, cy, cz, crx, cry, crz = get_transform(cam)
	
	crx = my
	cry = mx
	
	if w then
		cx, cy, cz = cx - (fx * dt * camspeed), cy + (fy * dt * camspeed), cz + (fz * dt * camspeed)
	end
	
	if s then
		cx, cy, cz = cx + (fx * dt * camspeed), cy - (fy * dt * camspeed), cz - (fz * dt * camspeed)
	end
	
	if j then
		add_force(cam, 0.0, 100.0, 0.0)
	end
	
	push_transform(cam, cx, cy, cz, crx, cry, crz)
end

function free()
	script_destroy(level)
end
