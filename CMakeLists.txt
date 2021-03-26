cmake_minimum_required(VERSION 3.19)

# Set project stuff
project(Engine VERSION 0.0.0)

# Files for lua
set(LUA_FILES
	lib/lua/src/lapi.c
	lib/lua/src/lcode.c
	lib/lua/src/lctype.c
	lib/lua/src/ldebug.c
	lib/lua/src/ldo.c
	lib/lua/src/ldump.c
	lib/lua/src/lfunc.c
	lib/lua/src/lgc.c
	lib/lua/src/llex.c
	lib/lua/src/lmem.c
	lib/lua/src/lobject.c
	lib/lua/src/lopcodes.c
	lib/lua/src/lparser.c
	lib/lua/src/lstate.c
	lib/lua/src/lstring.c
	lib/lua/src/ltable.c
	lib/lua/src/ltm.c
	lib/lua/src/lundump.c
	lib/lua/src/lvm.c
	lib/lua/src/lzio.c
	lib/lua/src/lauxlib.c
	lib/lua/src/lbaselib.c
	lib/lua/src/lcorolib.c
	lib/lua/src/ldblib.c
	lib/lua/src/liolib.c
	lib/lua/src/lmathlib.c
	lib/lua/src/loadlib.c
	lib/lua/src/loslib.c
	lib/lua/src/lstrlib.c
	lib/lua/src/ltablib.c
	lib/lua/src/lutf8lib.c
	lib/lua/src/linit.c
)

# Source files
set(FILES 
	${LUA_FILES}
	lib/glew/glew.c
	src/generic/world.c
	src/graphics/graphics.c
	src/graphics/opengl.c
	src/graphics/image.c
	src/io/fs.c
	src/io/load.c
	src/io/config.c
	src/io/input.c
	src/linux/main.c
	src/phys/phys.c
	src/util/alloc.c
	src/util/bag.c
	src/util/clocker.c
	src/util/thread.c
	src/util/flag.c
	src/util/time.c
	src/util/dstring.c
	src/util/fail.c
	src/util/maths.c
	src/util/rand.c
	src/util/array.c
	src/game.c
	src/systems.c
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

target_link_libraries(${EXEC_NAME} m)
target_link_libraries(${EXEC_NAME} GL)
target_link_libraries(${EXEC_NAME} glfw)
target_link_libraries(${EXEC_NAME} pthread)
