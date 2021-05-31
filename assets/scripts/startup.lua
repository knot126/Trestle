-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

cam = mgCamera()
frame = 0
ent = 0
next_pos = 100.0

function init()
	-- player init
	local player = new_cube(0.0, 1.5, 0.0, 1.0, 1.0, 1.0, true, QR_PHYS_ENABLE_RESPONSE)
	mgActivePlayer(player)
	
	buildWallsAndFloor(5.0, 8.0, 100.0)
	
	ent = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ent, "[text about in-game statistics will appear here shortly...]")
	mgUITextPos(ent, -1.0, -0.9)
	mgUITextSize(ent, 0.1)
	
	ui_LivesCountText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui_LivesCountText, "Lives Count")
	mgUITextPos(ui_LivesCountText, -0.95, 0.95)
	mgUITextSize(ui_LivesCountText, 0.06)
	
	ui_LivesCount = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui_LivesCount, "13")
	mgUITextPos(ui_LivesCount, -0.95, 0.88)
	mgUITextSize(ui_LivesCount, 0.09)
end

function tick()
	frame = frame + 1
	local x, y, z = mgPlayerPos()
	mgUIText(ent, "Frame " .. tostring(frame) .. ", " .. tostring(mgEntCount()) .. " Entities, Position: " .. "(" .. x .. ", " .. y .. ", " .. z .. ")")
	
	-- build more walls as the player moves forward
	if z < -next_pos then 
		buildWallsAndFloor(5.0, mgRandFloat() * 4.0 + 4.0, 100.0, next_pos)
		next_pos = next_pos + 100.0
	end
end
