-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Include script for gameplay scripts
-- 



-- glfw keys
GLFW_KEY_SPACE = 32
GLFW_KEY_A = 65
GLFW_KEY_D = 68
GLFW_KEY_S = 83
GLFW_KEY_W = 87
GLFW_KEY_ENTER = 257
GLFW_KEY_RIGHT = 262
GLFW_KEY_LEFT = 263
GLFW_KEY_DOWN = 264
GLFW_KEY_UP = 265

ENT_TRANSFORM = (1 << 0)
ENT_GRAPHICS_MESH = (1 << 1)
ENT_GRAPHICS_CURVE = (1 << 2)
ENT_PHYSICS_OBJECT = (1 << 3)
ENT_PHYSICS_SPHERE = (1 << 4)
ENT_PHYSICS_AABB = (1 << 5)
ENT_PHYSICS_MESH = (1 << 6)
ENT_GRAPHICS_SURFACE = (1 << 8)

PHYSICS_STATIC = (1 << 0)
PHYSICS_NO_GRAVITY = (1 << 1)
PHYSICS_DRAG = (1 << 2)
PHYSICS_MODE_PLAYER = (1 << 3)


function s_load_model(x, y, z, rx, ry, rz, s, model)
	-- In-script utility function for loading a model into an entity
	bm = create_entity(ENT_TRANSFORM | ENT_GRAPHICS_MESH)
	push_transform(bm, x, y, z, rx, ry, rz, s, s, s)
	push_obj_mesh(bm, model)
	return bm
end

actions = {}

function RegisterAction(name, keys)
	actions[name] = keys
end

function GetAction(name)
	for i = 0, #actions[name] do
		if get_key(actions[name][i]) then
			return true
		end
	end
	
	return false
end
