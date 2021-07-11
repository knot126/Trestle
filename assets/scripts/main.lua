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
	
	print("New entity name is ", e)
	
	push_transform(e, 1.0, 2.0, 3.0)
	set_camera(e)
end

function tick()
	
end

function free()
end
