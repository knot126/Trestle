set(FILES 
	graphics/graphics.c
	linux/main.c
	math/vector2.c
	math/vector3.c
	platform/fs.c
	util/alloc.c
	game.c
	main.c)

add_executable(${PROJECT_NAME} ${FILES})
