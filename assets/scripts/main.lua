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
	
	k = make_box(0.0, 0.0, -12.0, 8.0, 0.5, 12.0)
	create_aabb(k)
	set_aabb(k, 8.0, 0.5, 12.0)
	
	bm = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_MESH)
	push_transform(bm, -3.0, 0.0, -4.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25)
	push_obj_mesh(bm, "assets://mesh/gum_tree1.obj")
	
	bm2 = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_MESH)
	push_transform(bm2, 3.0, 0.0, -4.0, 0.0, 0.0, 0.0, 0.25, 0.25, 0.25)
	push_obj_mesh(bm2, "assets://mesh/gum_tree1.obj")
	
	player = make_box(0.0, 2.0, 0.0)
	create_physics_object(player)
	set_physics_flags(player, 0)
	create_aabb(player)
	set_aabb(player, 1.0, 1.0, 1.0)
	
	physics_sync_graph()
end

at = 0

function tick(dt)
	local w = get_key(GLFW_KEY_UP)
	local s = get_key(GLFW_KEY_DOWN)
	local a = get_key(GLFW_KEY_LEFT)
	local d = get_key(GLFW_KEY_RIGHT)
	
	-- unrelated
	local x, y, z, rx, ry, rz = get_transform(player)
	
	if at > 1.0 then
		print("player: (" .. x .. ", " .. y .. ", " .. z .. ")")
		at = 0
	end
	at = at + dt
	
	if w then
		add_force(player, 0.0, 0.0, -3.0)
	end
	
	if s then
		add_force(player, 0.0, 0.0, 3.0)
	end
	
	if a then
		add_force(player, -3.0, 0.0, 0.0)
	end
	
	if d then
		add_force(player, 3.0, 0.0, 0.0)
	end
end

function free()
end
