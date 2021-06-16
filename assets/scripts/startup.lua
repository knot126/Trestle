-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

cam = mgCamera(0.0, 0.0, 0.0, 0.0875, 0.0, 0.0)
frame = 0
lives = {}
ui = {}
next_pos = 100.0

function init()
	-- player init
	local player = new_cube(0.0, 1.5, 0.0, 0.9, 0.9, 0.9, true, QR_PHYS_ENABLE_RESPONSE, "assets://mesh/player.xml")
	mgActivePlayer(player)
	
	--buildWallsAndFloor(5.0, 8.0, 100.0)
	buildFloorTest(5.0, 12.0, 100.0)
	
	ui.Statistics = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Statistics, "[text about in-game statistics will appear here shortly...]")
	mgUITextPos(ui.Statistics, -1.0, -0.9)
	mgUITextSize(ui.Statistics, 0.1)
	
	ui.LivesCountText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.LivesCountText, "Lives")
	mgUITextPos(ui.LivesCountText, -0.95, 0.95)
	mgUITextSize(ui.LivesCountText, 0.06)
	mgUITextFont(ui.LivesCountText, "font3")
	
	ui.LivesCount = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.LivesCount, "quickrun")
	mgUITextPos(ui.LivesCount, -0.95, 0.88)
	mgUITextSize(ui.LivesCount, 0.09)
	mgUITextFont(ui.LivesCount, "font3")
	
	ui.DistanceText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.DistanceText, "Distance")
	mgUITextPos(ui.DistanceText, -0.75, 0.95)
	mgUITextSize(ui.DistanceText, 0.06)
	mgUITextFont(ui.DistanceText, "font3")
	
	ui.Distance = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Distance, "quickrun")
	mgUITextPos(ui.Distance, -0.75, 0.88)
	mgUITextSize(ui.Distance, 0.09)
	mgUITextFont(ui.Distance, "font3")
	
	-- mgBox(0.0, 0.0, -10.5, 1.0, 1.0, 1.0, mgRandFloat(), mgRandFloat(), mgRandFloat())
	
	lives.count = 5
	lives.updated = true
end

function tick()
	frame = frame + 1
	local x, y, z = mgPlayerPos()
	mgUIText(ui.Statistics, "Frame " .. tostring(frame) .. ", " .. tostring(mgEntCount()) .. " Entities, Position: " .. "(" .. x .. ", " .. y .. ", " .. z .. ")")
	mgUIText(ui.Distance, tostring(math.floor(-z)))
	
	-- build more walls as the player moves forward
	if z < -(next_pos - 20.0) then 
-- 		mgClearWorld()
		--buildWallsAndFloor(5.0, mgRandFloat() * 4.0 + 4.0, 100.0, next_pos)
		buildFloorTest(5.0, 12.0, 100.0, next_pos)
		next_pos = next_pos + 100.0
	end
	
	if lives.updated then
		mgUIText(ui.LivesCount, tostring(lives.count))
		lives.updated = false
	end
	
	if y < -1.5 then
		mgResetPlayer()
		lives.updated = true
		lives.count = lives.count - 1
	end
	
	if lives.count < 0 and not lives.hasBeenDead then
		lives.hasBeenDead = true
		ui.Dead = mgUIElement(QR_ELEMUI_TEXT)
		mgUIText(ui.Dead, "Player dead !")
		mgUITextPos(ui.Dead, -0.95, 0.25)
		mgUITextSize(ui.Dead, 0.5)
		mgUITextFont(ui.Dead, "font3")
		mgPaused(true)
	end
end
