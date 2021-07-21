-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Include script for gameplay scripts
-- 



-- glfw keys
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

PHYSICS_STATIC = (1 << 0)
PHYSICS_NO_GRAVITY = (1 << 1)
PHYSICS_DRAG = (1 << 2)
PHYSICS_MODE_PLAYER = (1 << 3)
