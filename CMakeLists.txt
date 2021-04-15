cmake_minimum_required(VERSION 3.19)

# Set project
project(Engine VERSION 0.1.0)

set(EXEC_NAME quickrun)
set(MELON_NAME melon)

# Find OpenGL
include("FindOpenGL")

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

# Melon Source files
set(MELON_FILES
	src/util/alloc.c
	src/util/bag.c
	src/util/thread.c
	src/util/flag.c
	src/util/time.c
	src/util/string.c
	src/util/fail.c
	src/util/maths.c
	src/util/rand.c
	src/util/array.c
	src/util/strmap.c
	src/util/xml.c
	src/util/fs.c
	src/util/load.c
	src/util/config.c)

# Source files
set(EXEC_FILES 
	${LUA_FILES}
	${MELON_FILES}
	lib/glew/glew.c
	src/world/world.c
	src/world/transform.c
	src/world/compo/graphics.c
	src/graphics/graphics.c
	src/graphics/opengl.c
	src/graphics/image.c
	src/input/input.c
	src/linux/main.c
	src/phys/phys.c
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
add_executable(${EXEC_NAME} ${EXEC_FILES})
add_library(${MELON_NAME} SHARED ${MELON_FILES})

target_link_libraries(${EXEC_NAME} m)
target_link_libraries(${EXEC_NAME} GL)
target_link_libraries(${EXEC_NAME} glfw)
target_link_libraries(${EXEC_NAME} pthread)
