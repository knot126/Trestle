function init()
	print("Init called!")
	print("")
	print("You are using the Trestle test project! If you mean to load a game, plese make")
	print("sure you have used ./[exec name] --assets [path to folder]")
end

function tick()
	graphics_update()
end

function free()
	print("Free called!")
end
