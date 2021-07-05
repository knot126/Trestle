tk = 0

function init()
	mgPaused(true)
	
	adsfadsfadsf = mgUIElement(QR_ELEMUI_TEXT)
	mgUIText(adsfadsfadsf, "Press Start to Play!!")
	mgUITextPos(adsfadsfadsf, 0.0 - (0.025 * 11), -0.05)
	mgUITextSize(adsfadsfadsf, 0.1)
	mgUITextColour(adsfadsfadsf, 1.0, 1.0, 1.0, 0.8)
end

function tick()
	tk = tk + 1
	
	if tk > 500 then
		mgClearWorld()
		mgYeildToScript("assets://scripts/startup.lua")
	end
end

function free()
end
