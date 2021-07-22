-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

function init()
	set_background(mgRandFloat(), mgRandFloat(), mgRandFloat(), 1.0)
	enable_physics(true)
	
	cam = create_entity(ENT_TRANSFORM)
	push_transform(cam, 0.0, 5.0, 5.65, 0.0375, 0.0, 0.0)
	set_camera(cam)
	
	bm = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_MESH)
	push_transform(bm, -3.0, 0.0, -4.0, 0.0, 0.0, -0.05, 0.25, 0.25, 0.25)
	push_obj_mesh(bm, "assets://mesh/gum_tree1.obj")
	
	bm2 = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_MESH)
	push_transform(bm2, 3.0, 0.0, -4.0, 0.0, 0.0, 0.01, 0.25, 0.25, 0.25)
	push_obj_mesh(bm2, "assets://mesh/gum_tree1.obj")
	
	player = make_box(0.0, 2.0, -2.0)
	set_physics_flags(player, PHYSICS_MODE_PLAYER)
	
	k = make_box(0.0, 0.0, -36.0, 8.0, 0.5, 36.0)
	
	physics_sync_graph()
end

at = 0
speed = 2.0

function tick(dt)
	local w = get_key(GLFW_KEY_UP)
	local s = get_key(GLFW_KEY_DOWN)
	local a = get_key(GLFW_KEY_LEFT)
	local d = get_key(GLFW_KEY_RIGHT)
	local j = get_key(GLFW_KEY_SPACE)
	
	-- unrelated
	local x, y, z, rx, ry, rz = get_transform(player)
	
	if at > 1.0 then
		print("Player position: (" .. x .. ", " .. y .. ", " .. z .. ")")
		print("Speed: " .. speed)
		at = 0
	end
	at = at + dt
	
	direct_move(player, 0.0, 0.0, -1000.0 * speed * dt)
	
	if w then
		speed = speed + 1.0 * dt
	end
	
	if s then
		speed = speed - 1.0 * dt
	end
	
	if a then
		direct_move(player, -3.0, 0.0, 0.0)
	end
	
	if d then
		direct_move(player, 3.0, 0.0, 0.0)
	end
	
	if j then
		add_force(player, 0.0, 17.0, 0.0)
	end
	
	local cx, cy, cz, crx, cry, crz = get_transform(cam)
	
	cx = x * 0.5
	cy = y + 4.0
	cz = z + 8.0
	
	push_transform(cam, cx, cy, cz, crx, cry, crz)
end

function free()
end
