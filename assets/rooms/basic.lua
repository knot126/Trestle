function init()
	mgSetSpeed(10.0, 15.0)
	
	mgBox(0.0, 0.0, -16.0, 0.5, 0.5, 32.0)
	mgBox(1.75, 0.0, -48.0, 0.5, 0.5, 32.0)
	mgBox(1.75, -0.5, -48.0, 0.5, 0.5, 32.0)
	mgBox(-2.5, 0.0, -72.0, 0.5, 0.5, 32.0)
	mgBox(-2.0, 0.8, -48.0, 0.5, 0.5, 32.0)
	mgBox(0.0, 0.0, -96.0, 0.5, 0.5, 32.0)
	mgBox(-6.0, -3.0, -64.0, 3.0, 0.5, 128.0)
	mgBox(6.0, -3.0, -64.0, 3.0, 0.5, 64.0)
	
	for i = 0, 40, 1 do
		x = math.floor((mgRandFloat() - 0.5) * 24.0)
		y = math.floor((mgRandFloat() - 0.5) * 24.0)
		z = math.floor((mgRandFloat() - 0.5) * 256.0)
-- 		mgBox(x, y, z)
		loadModel(x, 0.0, z, 0.2, 0.2, 0.2, "assets://mesh/gum_tree1.obj")
	end
	
	mgLength(128)
end

function tick() end
function free() end
