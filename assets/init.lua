function init()
	script = script_open("assets://scripts/main.lua")
end

function tick()
	-- Don't not have tick here, the script system doesn't like it and just
	-- starts writing arbitrary memory for some reason. I will try to fix but
	-- it's an issue that I don't want to work on right now.
end

function free()
	script_destroy(script)
end
