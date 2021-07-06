tk = 0
a = {} -- menu/interactable
ui = {} -- non-interactable

function init()
	mgPaused(true)
	
	ui.QuickRunText = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(ui.QuickRunText, "Quick Run!")
	mgUITextPos(ui.QuickRunText, -0.9, 0.8)
	mgUITextSize(ui.QuickRunText, 0.2)
	mgUITextColour(ui.QuickRunText, 1.0, 1.0, 1.0, 0.8)
	
	a[1] = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(a[1], "Play Game")
	mgUITextPos(a[1], -0.9, 0.45)
	mgUITextSize(a[1], 0.1)
	mgUITextColour(a[1], 1.0, 1.0, 1.0, 0.8)
	
	a[2] = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(a[2], "Options")
	mgUITextPos(a[2], -0.9, 0.3)
	mgUITextSize(a[2], 0.1)
	mgUITextColour(a[2], 1.0, 1.0, 1.0, 0.8)
	
	active = 1
	
	-- update
	for i = 1, #a, 1 do
		if i == active then
			mgUITextColour(a[i], 0.0, 1.0, 0.0, 1.0)
		else
			mgUITextColour(a[i], 1.0, 1.0, 1.0, 1.0)
		end
	end
end

function tick()
	tk = tk - 1
	
	-- get the keys
	local up = mgGetKey(265)
	local down = mgGetKey(264)
	
	-- update active item based on keypress
	if (up or down) and tk < 1 then
		if up then
			active = active - 1
		elseif down then
			active = active + 1
		end
		tk = 100
		updated = true
	end
	
	-- bounds checking for table
	if active > #a then
		active = #a
	end
	
	if active < 1 then
		active = 1
	end
	
	-- update main menu colour
	if updated then
		for i = 1, #a, 1 do
			if i == active then
				mgUITextColour(a[i], 0.0, 1.0, 0.0, 1.0)
			else
				mgUITextColour(a[i], 1.0, 1.0, 1.0, 1.0)
			end
		end
	end
	
	-- do the stuff
	if mgGetKey(257) then
		-- Play game button
		if active == 1 then
			mgClearWorld()
			mgYeildToScript("assets://scripts/startup.lua")
		elseif active == 2 then
			print("Not implemented!!!")
		end
	end
	
	-- reset updated feild
	updated = false
end

function free()
end
