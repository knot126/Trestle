set(FILES 
	graphics.c assoc.c main.c linux/main.c 
	math/vector3.c math/vector2.c)

add_executable(${PROJECT_NAME} ${FILES})
