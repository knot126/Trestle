-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

-- cubes = {}
-- for z = 0.0, 16.0, 1.0 do
-- 	cubes[#cubes + 1] = new_cube(
-- 		(mgRandFloat() - 0.5) * 5.0, (mgRandFloat() - 0.5) * 5.0, -z,
-- 		mgRandFloat(), mgRandFloat(), mgRandFloat(),
-- 		true, nil)
-- 	--print("Crated cube with entity ID ", ent)
-- end

cam = mgCamera()

-- a = new_cube(0.0, 0.0, -16.0, 5.0, 0.5, 32.0, false, nil)
-- b = new_cube(-2.5, 2.0, -16.0, 0.5, 4.0, 32.0, false, nil)
-- c = new_cube(2.5, 2.0, -16.0, 0.5, 4.0, 32.0, false, nil)

function init()
	-- player init
	player = new_cube(0.0, 1.5, 0.0, 1.0, 1.0, 1.0, true, QR_PHYS_ENABLE_RESPONSE)
	mgActivePlayer(player)
	
	buildWallsAndFloor(5.0, 8.0, 100.0)
end

init()
