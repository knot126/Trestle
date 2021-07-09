-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

cam = mgCamera(0.0, 0.0, 0.0, 0.0875, 0.0, 0.0)
frame = 0
first = 0
lives = {}
ui = {}
next_pos = 100.0

function initHud()
	ui.Statistics = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Statistics, "[text about in-game statistics will appear here shortly...]")
	mgUITextPos(ui.Statistics, -1.0, -0.9)
	mgUITextSize(ui.Statistics, 0.1)
	mgUITextColour(ui.Statistics, 0.0, 1.0, 0.3, 1.0)
	
	ui.LivesCountText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.LivesCountText, "Lives")
	mgUITextPos(ui.LivesCountText, -0.95, 0.95)
	mgUITextSize(ui.LivesCountText, 0.06)
	
	ui.LivesCount = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.LivesCount, "quickrun")
	mgUITextPos(ui.LivesCount, -0.95, 0.88)
	mgUITextSize(ui.LivesCount, 0.09)
	
	ui.DistanceText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.DistanceText, "Distance")
	mgUITextPos(ui.DistanceText, -0.75, 0.95)
	mgUITextSize(ui.DistanceText, 0.06)
	
	ui.Distance = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Distance, "quickrun")
	mgUITextPos(ui.Distance, -0.75, 0.88)
	mgUITextSize(ui.Distance, 0.09)
	
	ui.SpeedText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.SpeedText, "Speed")
	mgUITextPos(ui.SpeedText, -0.55, 0.95)
	mgUITextSize(ui.SpeedText, 0.06)
	
	ui.Speed = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.Speed, "quickrun")
	mgUITextPos(ui.Speed, -0.55, 0.88)
	mgUITextSize(ui.Speed, 0.09)
	
	ui.ProgressBarBG = mgUIElement(QR_ELEMUI_BOX)
	mgUIBox(ui.ProgressBarBG, -1.0, 1.0, 2.0, 0.025)
	mgUIBoxColour(ui.ProgressBarBG, 0.0, 0.0, 0.0, 0.75)
	
	ui.ProgressBar = mgUIElement(QR_ELEMUI_BOX)
	mgUIBox(ui.ProgressBar, -1.0, 1.0, 1.0, 0.025)
	mgUIBoxColour(ui.ProgressBar, 1.0, 1.0, 1.0, 1.0)
end

function init()
	-- player init
	local player = new_cube(0.0, 1.5, 0.0, 0.9, 0.9, 0.9, true, QR_PHYS_ENABLE_RESPONSE, "assets://mesh/player.obj")
	mgActivePlayer(player)
	
	lives.count = 5
	lives.updated = true
	
	initHud()
	
	first = mgEntity(0)
end

function room()
	return next_pos / 100.0
end

function colour(n)
	local fn = math.floor(n)
	if fn % 2 == 0 then
		return n - fn
	else
		return -(n - fn) + 1.0
	end
end

function tickHud(x, y, z, frame)
	mgUIText(ui.Statistics, "Frame " .. tostring(frame) .. ", " .. tostring(mgEntCount()) .. " Entities, Position: " .. "(" .. x .. ", " .. y .. ", " .. z .. ")")
	mgUIText(ui.Distance, tostring(math.floor(-z)))
	mgUIText(ui.Speed, tostring(mgGetSpeed()))
	
	mgUIBox(ui.ProgressBar, -1.0, 1.0, -z * 0.002, 0.025)
end

function tick()
	frame = frame + 1
	local x, y, z = mgPlayerPos()
	
	tickHud(x, y, z, frame)
	
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
-- 		ui.Dead = mgUIElement(QR_ELEMUI_TEXT)
-- 		mgUIText(ui.Dead, "Player dead !")
-- 		mgUITextPos(ui.Dead, -0.85, 0.25)
-- 		mgUITextSize(ui.Dead, 0.35)
-- 		mgUITextFont(ui.Dead, "font3")
-- 		mgPaused(true)
	end
	
	-- Pulse the text colour
	if lives.hasBeenDead then
-- 		mgUITextColour(ui.Dead, 1.0, (math.sin(frame * 0.01) + 1.0) / 2.0, (math.sin(frame * 0.01) + 1.0) / 2.0, 1.0)
		if mgGetKey(GLFW_KEY_ENTER) then
			mgClearWorld()
			mgYeildToScript("assets://scripts/menu.lua")
		end
	end
end

function free()
end
