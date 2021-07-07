function init()
	--mgPaused(true)
	
	gr = crab:addOption(-0.9, 0.6, "Graphics Quality", {"Low", "Medium", "High", "Ultra"})
	wasd = crab:addOption(-0.9, 0.45, "Enable Wasd", {"Yes", "No"})
	dbg = crab:addOption(-0.9, 0.3, "Debug Features", {"ON", "OFF"})
end

function tick()
	crab:update()
	print(crab:getValue(gr), crab:getValue(wasd))
end

function free()
end
