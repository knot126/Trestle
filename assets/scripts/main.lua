-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

cam = mgCamera(0.0, 0.0, 0.0, 0.0875, 0.0, 0.0)
first = 0

function init()
	-- player init
	local player = loadModel(0.0, 1.5, 0.0, 0.9, 0.9, 0.9, true, QR_PHYS_ENABLE_RESPONSE, "assets://mesh/player.obj")
	mgActivePlayer(player)
	
	first = mgEntity(0)
end

function tick()
	
end

function free()
end
