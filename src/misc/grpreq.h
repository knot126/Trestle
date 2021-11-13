#pragma once 

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#if defined(DG_INCLUDE_NATIVE_WINDOW_API)
	#if defined(__linux__)
		#define GLFW_EXPOSE_NATIVE_X11
		#define GLFW_EXPOSE_NATIVE_WAYLAND
	#endif
	#include <GLFW/glfw3native.h>
#endif
