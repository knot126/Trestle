r, g, b = mgRandFloat(), mgRandFloat(), mgRandFloat()

space = {}

function doRoomIteration()
	
end

function init()
	set_background(r, g, b, 1.0)
	
	make_box(0.0, -0.5, 0.0, 32.0, 0.5, 32.0, r - 0.1, g - 0.1, b - 0.1, "tile1")
end

function tick(dt)
	
end
