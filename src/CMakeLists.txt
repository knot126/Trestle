set(FILES 
	graphics.c
	main.c
	game.c
	alloc.c
	linux/main.c
	math/vector3.c)

add_executable(${PROJECT_NAME} ${FILES})
