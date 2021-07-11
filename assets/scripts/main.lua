-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

ENT_TRANSFORM = (1 << 0)
ENT_GRAPHICS_MESH = (1 << 1)
ENT_GRAPHICS_CURVE = (1 << 2)

function init()
	e = create_entity(ENT_TRANSFORM)
	push_transform(e, 0.0, 4.0, 5.65, 0.0875, 0.0, 0.0)
	set_camera(e)
	
	print("New entity name is ", e)
	
	k = make_box(0.0, 0.0, -12.0, 8.0, 0.5, 12.0)
	
	bm = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_MESH)
	push_transform(bm, 0.0, 0.0, -4.0)
	push_obj_mesh(bm, "assets://mesh/gum_tree1.obj")
end

function tick()
	local up = get_key(GLFW_KEY_UP)
	local down = get_key(GLFW_KEY_DOWN)
	
	if up then
		local x, y, z = get_transform(e)
		y = y + 0.1
		push_transform(e, x, y, z)
	end
	
	if down then
		local x, y, z = get_transform(e)
		y = y - 0.1
		push_transform(e, x, y, z)
	end
end

function free()
end
