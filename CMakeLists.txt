cmake_minimum_required(VERSION 3.18)

# Set project
project(Engine VERSION 0.1.1)

set(EXEC_NAME quickrun)
set(MELON_NAME melon)

# Find OpenGL
include("FindOpenGL")

# Include lua
set(LUA_DIR "lib/lua/src")

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

set(LUA_INCLUDE_FILES
	${LUA_DIR}/lapi.h
	${LUA_DIR}/ldebug.h
	${LUA_DIR}/ljumptab.h
	${LUA_DIR}/lobject.h
	${LUA_DIR}/lprefix.h
	${LUA_DIR}/ltm.h
	${LUA_DIR}/lundump.h
	${LUA_DIR}/lauxlib.h
	${LUA_DIR}/ldo.h
	${LUA_DIR}/llex.h
	${LUA_DIR}/lopcodes.h
	${LUA_DIR}/lstate.h
	${LUA_DIR}/lua.h
	${LUA_DIR}/lvm.h
	${LUA_DIR}/lcode.h
	${LUA_DIR}/lfunc.h
	${LUA_DIR}/llimits.h
	${LUA_DIR}/lopnames.h
	${LUA_DIR}/lstring.h
	${LUA_DIR}/luaconf.h
	${LUA_DIR}/lzio.h
	${LUA_DIR}/lctype.h
	${LUA_DIR}/lgc.h
	${LUA_DIR}/lmem.h
	${LUA_DIR}/lparser.h
	${LUA_DIR}/ltable.h
	${LUA_DIR}/lualib.h
)

# Melon Source files
set(MELON_FILES
	"src/util/alloc.c"
	"src/util/args.c"
	"src/util/bezsurfa.c"
	"src/util/surface.c"
	"src/util/bitmap.c"
	"src/util/fs.c"
	"src/util/hash.c"
	"src/util/log.c"
	"src/util/maths.c"
	"src/util/obj.c"
	"src/util/rand.c"
	"src/util/script.c"
	"src/util/str.c"
	"src/util/stream.c"
	"src/util/thread.c"
	"src/util/time.c"
	"src/util/value.c"
	${LUA_INCLUDE_FILES}
)

# Source files
set(EXEC_FILES 
	${LUA_FILES}
	${MELON_FILES}
	
	"src/game/box.c"
	"src/game/gamescript.c"
	"src/game/rect.c"
	"src/game/scripting.c"
	"src/game/scriptman.c"
	"src/global/reg.c"
	"src/global/supervisor.c"
	"src/graph/graph.c"
	"src/graphics/extern/glad.c"
	"src/graphics/mesh.c"
	"src/graphics/graphics.c"
	"src/graphics/texture.c"
	"src/graphics/image.c"
	"src/input/input.c"
	"src/input/glfwi.c"
	"src/physics/physics.c"
	"src/resman/resman.c"
	"src/vulkan/core.c"
	"src/window/window.c"
	"src/game.c"
	"src/main.c"
	"src/test.c"
	"src/types.c"
)

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

# Include dirs for built-in libraries
target_include_directories(${EXEC_NAME} PRIVATE ${LUA_DIR})
target_include_directories(${MELON_NAME} PRIVATE ${LUA_DIR})

target_include_directories(${EXEC_NAME} PRIVATE "src")
target_include_directories(${EXEC_NAME} PRIVATE "lib/glfw/include")
target_include_directories(${EXEC_NAME} PRIVATE "lib/Vulkan-Headers/include")

# Link options for EXEC
target_link_libraries(${EXEC_NAME} m)
target_link_libraries(${EXEC_NAME} vulkan)
target_link_libraries(${EXEC_NAME} dl)
target_link_libraries(${EXEC_NAME} glfw)
target_link_libraries(${EXEC_NAME} pthread)
target_link_libraries(${EXEC_NAME} pulse)
target_link_libraries(${EXEC_NAME} pulse-simple)
