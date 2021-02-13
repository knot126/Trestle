cmake_minimum_required(VERSION 3.19)

# Set project stuff
project(Engine VERSION 0.0.0)

# Files for lua
set(LUA_FILES
	src/lua/lapi.c
	src/lua/lctype.c
	src/lua/lfunc.c
	src/lua/lmathlib.c
	src/lua/loslib.c
	src/lua/ltable.c
	src/lua/lundump.c
	src/lua/lauxlib.c
	src/lua/ldblib.c
	src/lua/lgc.c
	src/lua/lmem.c
	src/lua/lparser.c
	src/lua/ltablib.c
	src/lua/lutf8lib.c
	src/lua/lbaselib.c
	src/lua/ldebug.c
	src/lua/linit.c
	src/lua/loadlib.c
	src/lua/lstate.c
	src/lua/ltm.c
	src/lua/lvm.c
	src/lua/lcode.c
	src/lua/ldo.c
	src/lua/liolib.c
	src/lua/lobject.c
	src/lua/lstring.c
	src/lua/lua.c
	src/lua/lzio.c
	src/lua/lcorolib.c
	src/lua/ldump.c
	src/lua/llex.c
	src/lua/lopcodes.c
	src/lua/lstrlib.c
)

# Source files
set(FILES 
#	${LUA_FILES}
	src/glew/glew.c
	src/graphics/graphics.c
	src/graphics/vulkan.c
	src/graphics/opengl.c
	src/io/fs.c
	src/io/load.c
	src/linux/main.c
	src/math/vector2.c
	src/math/vector3.c
	src/phys/phys.c
	src/util/alloc.c
	src/util/bag.c
	src/util/thread.c
	src/util/flag.c
	src/util/time.c
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
target_link_libraries(${EXEC_NAME} pthread)
