/* Polar "game engine" (not really yet, but soon!)
 * Prepare for sipcy comments! */

// GLFW becuase that makes things simpiler for now. I'd rather avoid it in the
// future, but oh well.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring> // needed for string compare functions
#include <vector>

// Window width and height
const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

// Minimum vulkan level to log
const VkDebugUtilsMessageTypeFlagsEXT VULKAN_MIN_MESSAGE_LEVEL = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

// Global state for graphics
/* I chose not to use classes for this as it doesn't make sense (imo) to have
 * a class to manage the entire application. Maybe it feel like it's in a class
 * and isolated, but if the class is the entire program, what is the purpose?
 * Perhaps it works better in a real game where you have many state, but not 
 * here. */
GLFWwindow* glfw_window;
VkInstance vulkanInstance;
VkDebugUtilsMessengerEXT vulkanDebugMessenger;

// ValidationLayers related things
const std::vector<const char*> GS_validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
) {
	if (message_severity >= VULKAN_MIN_MESSAGE_LEVEL) {
		std::cerr << "[VULKAN] " << pCallbackData->pMessage << std::endl;
	}
	
	return VK_FALSE;
}

void setupDebugMessenger() {
	if (!enableValidationLayers) {
		return;
	}
	
	VkDebugUtilsMessengerCreateInfoEXT create_info;
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = 
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | 
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info.messageType = 
	VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
	VK_DEBUG_UTILS_MESSAGE_TYPE_PREFORMANCE_BIT_EXT;
	create_info.pfnUserCallback = debugCallback;
	create_info.pUserData = nullptr;
}

std::vector<const char*> getRequiredExtensions() {
	/* Return all of the required extensions */
	
	uint32_t glfw_extension_count;
	const char** glfw_extensions;
	
	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
	
	std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
	
	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	
	return extensions;
}

bool CheckValidationLayersSupport() {
	/* Check for validation layer support */
	
	std::cout << "Checking for validation layer support." << std::endl;
	
	// Get layer properties
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	
	// A second time to store the actual data
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	// Check for the validation layer
	for (const char* layer : GS_validationLayers) {
		bool layerFound = false;
		
		for (const auto& properties : availableLayers) {
			if (!strcmp(layer, properties.layerName)) {
				layerFound = true;
				break;
			}
		}
		
		if (!layerFound) {
			return false;
		}
	}
	
	return true;
}

void CreateInstance() {
	/* Create vulkan instance
	 * This function will create the vulkan instance and initialize GLFW
	 * needs. It starts by filling out some structs and then gets into 
	 * checking that all of the extensions that are need are here. */
	
	// Check for vaildation layers
	if (enableValidationLayers && !CheckValidationLayersSupport()) {
		std::cout << "Validation layers were requested but were not found." << std::endl;
		exit(1);
	} else {
		std::cout << "Validation layer support is working or disabled." << std::endl;
	}
	
	// Create information structs
	VkApplicationInfo appInfo;
	
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "Polar Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	
	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
	// Get required extensions
	std::vector<const char*> extensions = getRequiredExtensions();
	
	// CreateInfo struct setup
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(GS_validationLayers.size());
		createInfo.ppEnabledLayerNames = GS_validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}
	createInfo.pNext = nullptr;
	
	// Get available extensions
	// Get only the count the first time, so our vector is an okay size
	uint32_t extensionsCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);
	
	// Now store the properties
	std::vector<VkExtensionProperties> extension_properties(extensionsCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extension_properties.data());
	
	// Print status message about how many strings are available
	std::cout << "The following extensions are available:\n";
	for (const auto& extension : extension_properties) {
		std::cout << "\t" << extension.extensionName << "\n";
	}
	
	// Create instance
	VkResult result = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
	
	if (result != VK_SUCCESS) {
		std::cout << "Error! Vulkan status: " << result << std::endl;
		exit(1);
	}
}

void InitVulkan() {
	CreateInstance();
	setupDebugMessenger();
}

void InitWindow() {
	glfwInit();
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	glfw_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
}
	
void MainLoop() {
	while (!glfwWindowShouldClose(glfw_window)) {
		glfwPollEvents();
	}
}

void Cleanup() {
	vkDestroyInstance(vulkanInstance, nullptr);
	
	glfwDestroyWindow(glfw_window);
	
	glfwTerminate();
}

int main() {
	InitVulkan();
	InitWindow();
	MainLoop();
	Cleanup();
	
	return EXIT_SUCCESS;
}
