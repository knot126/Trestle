-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

function init()
	local status = mgGetEngineState()
	
	if not status then
		mgSet("object.property", "test")
	end
end
