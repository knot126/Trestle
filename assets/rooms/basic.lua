theme = {r = 0.25, g = 0.65, b = 0.175}

function box(x, y, z, sx, sy, sz)
	return mgBoxLevel(x, y, z, sx, sy, sz, theme.r, theme.g, theme.b, "tile1")
end

t = 0.0

function pz()
	t = t + 0.001
	return math.sin(t) - 0.5
end

function init()
	mgSetSpeed(10.0, 15.0)
	
	mgPaused(true)
	
-- 	for i = 0, 1000 do
-- 		box(pz() * 16.0, pz() * 8.0, pz() * 140.0, 1.0, 1.0, 1.0)
		--mgObjMesh(t, "assets://mesh/player.obj")
-- 	end
	
	mgLength(140)
end

function tick() end
function free() end
