toruses = {}

r, g, b = mgRandFloat(), mgRandFloat(), mgRandFloat()
br, bg, bb = r, g, b
-- r, g, b = 1.0, 1.0, 1.0

function RandPosAxis(max, diss)
	a = (mgRandFloat() - 0.5) * 2.0 * max
	if a > diss or a < -diss then
		return a
	elseif a > 0.0 then
		return diss
	else
		return -diss
	end
end

function RandNegate(n)
	if rand_bool() then
		return n
	else
		return -n
	end
end

function GenerateLevel()
	--[[
	Generate the entire level 
	]]--
	
	make_box(0.0, -0.5, -36.0, 8.0, 0.5, 36.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	
	make_box(-9.0, 0.0, -36.0, 1.0, 2.0, 36.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	make_box(9.0, 0.0, -36.0, 1.0, 2.0, 36.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	
	-- Load some tree models for the forest
	for z = 0.0, 150.0, 12.0 do
		s_load_model(RandPosAxis(16.0, 8.0), 0.0, -z, 0.0, 0.0, 0.0, 0.2, "assets://mesh/gum_tree1.obj")
	end
	
	toruses[1] = s_load_model(2.5, 1.0, -48.0, mgRandFloat(), mgRandFloat(), mgRandFloat(), 2.0, "assets://mesh/torus.obj")
	toruses[2] = s_load_model(-2.5, 1.0, -32.0, mgRandFloat(), mgRandFloat(), mgRandFloat(), 2.0, "assets://mesh/torus.obj")
	
	for l = 72.0, 250.0, 1.0 do
		make_box(math.sin(l) * 8.0, math.cos(l) * 8.0, -l, 2.0, 1.0, 2.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	end
	
	for l = 0.0, 72.0, 12.0 do
		make_box(RandNegate(5.0), 0.0, -l, 1.0, 16.0, 1.0, r - 0.1, g - 0.1, b - 0.1, "tile0")
	end
end

function UpdateRotation(e, dt)
	local x, y, z, rx, ry, rz, sx, sy, sz = get_transform(e)
	
	rx = rx + dt * mgRandFloat() * 2.0
	ry = ry + dt * mgRandFloat() * 2.0
	rz = rz + dt * mgRandFloat() * 2.0
	
	push_transform(e, x, y, z, rx, ry, rz, sx, sy, sz)
end

function init()
	-- Set the background colour
	set_background(br, bg, bb, 1.0)
	
	-- Generate the level
	GenerateLevel()
end

function tick(dt)
	-- update the rotations
	UpdateRotation(toruses[1], dt)
	UpdateRotation(toruses[2], dt)
end
