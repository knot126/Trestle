cmake_minimum_required(VERSION 3.19)

# Set project stuff
project(Engine VERSION 0.0.0)

# Source files
set(FILES 
	src/graphics/graphics.c
	src/graphics/vulkan.c
	src/graphics/opengl.c
	src/glew/glew.c
	src/linux/main.c
	src/math/vector2.c
	src/math/vector3.c
	src/io/fs.c
	src/util/alloc.c
	src/util/bag.c
#	src/util/dictionary.c
#	src/util/generic.c
	src/game.c
	src/types.c
	src/main.c)

# Force compiler to high warning levels
if(MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Create executable and set up linking
set(EXEC_NAME quickrun)

add_executable(${EXEC_NAME} ${FILES})

target_include_directories(${EXEC_NAME} PUBLIC /home/user/Development/vulkan-sdk/1.2.162.1/x86_64/include/)
target_link_libraries(${EXEC_NAME} m)
target_link_libraries(${EXEC_NAME} vulkan)
target_link_libraries(${EXEC_NAME} GL)
target_link_libraries(${EXEC_NAME} glfw)
