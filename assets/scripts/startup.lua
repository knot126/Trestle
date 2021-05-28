-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

cam = mgCamera()
frame = 0
ent = 0

function init()
	-- player init
	local player = new_cube(0.0, 1.5, 0.0, 1.0, 1.0, 1.0, true, QR_PHYS_ENABLE_RESPONSE)
	mgActivePlayer(player)
	
	buildWallsAndFloor(5.0, 8.0, 100.0)
	
	ent = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ent, "Quick Run, version 0.1.1")
	mgUITextPos(ent, -1.0, 1.0)
	mgUITextSize(ent, 0.1)
end

function tick()
	frame = frame + 1
	mgUIText(ent, "Frame " .. tostring(frame) .. ", " .. tostring(mgEntCount()) .. " Entities")
end
