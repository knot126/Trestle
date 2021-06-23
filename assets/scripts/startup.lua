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
	
	mgSetSpeed(1.0, 10.0)
	
	-- spawn
	mgBox(
		0.0, 0.0, -8.0,
		6.0, 0.25, 8.0,
		mgRandFloat(), mgRandFloat(), mgRandFloat(), "tile0"
	)
	
	buildFloorTest(5.0, 12.0, 100.0)
	
	ui.Statistics = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Statistics, "[text about in-game statistics will appear here shortly...]")
	mgUITextPos(ui.Statistics, -1.0, -0.9)
	mgUITextSize(ui.Statistics, 0.1)
	mgUITextColour(ui.Statistics, 0.0, 1.0, 0.3, 1.0)
	
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
	
	ui.SpeedText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.SpeedText, "Speed")
	mgUITextPos(ui.SpeedText, -0.55, 0.95)
	mgUITextSize(ui.SpeedText, 0.06)
	mgUITextFont(ui.SpeedText, "font3")
	
	ui.Speed = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Speed, "quickrun")
	mgUITextPos(ui.Speed, -0.55, 0.88)
	mgUITextSize(ui.Speed, 0.09)
	mgUITextFont(ui.Speed, "font3")
	
	lives.count = 5
	lives.updated = true
end

function room()
	return next_pos / 100.0
end

function tick()
	frame = frame + 1
	local x, y, z = mgPlayerPos()
	mgUIText(ui.Statistics, "Frame " .. tostring(frame) .. ", " .. tostring(mgEntCount()) .. " Entities, Position: " .. "(" .. x .. ", " .. y .. ", " .. z .. ")")
	mgUIText(ui.Distance, tostring(math.floor(-z)))
	mgUIText(ui.Speed, tostring(mgGetSpeed()))
	
	-- build more walls as the player moves forward
	if z < -(next_pos - 20.0) then 
		buildFloorTest(5.0, 12.0, 100.0, next_pos)
		next_pos = next_pos + 100.0
		mgSetSpeed((room() / 10.0) + 1.0, room() + 10.0)
	end
	
	if lives.updated then
		mgUIText(ui.LivesCount, tostring(lives.count))
		lives.updated = false
	end
	
	if y < -1.5 then
		mgResetPlayer()
		lives.updated = true
		lives.count = lives.count - 1
		
		-- respawn area
		mgBox(0.0, 0.0, -8.0 + z + 10.0, 6.0, 0.25, 8.0, mgRandFloat(), mgRandFloat(), mgRandFloat(), "tile0")
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
	
	if lives.hasBeenDead then
		mgUITextColour(ui.Dead, 1.0, (math.sin(frame * 0.01) + 1.0) / 2.0, (math.sin(frame * 0.01) + 1.0) / 2.0, 1.0)
	end
end
