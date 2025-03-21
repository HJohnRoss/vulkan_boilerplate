#ifndef TRIANGLE_H
#define TRIANGLE_H


/*#include <vulkan/vulkan.h>*/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
/*#include <cglm/cglm.h>*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600

extern const char *validationLayers[];
extern const size_t validationLayerCount;
extern VkDebugUtilsMessengerEXT debugMessenger;
extern VkPhysicalDevice physicalDevice;
	
#ifndef NODEBUG
    #define ENABLE_VALIDATION_LAYERS 1  // Enable validation layers in debug mode
#else 
    #define ENABLE_VALIDATION_LAYERS 0  // Disable validation layers in release mode
#endif
GLFWwindow *window;
VkInstance instance;

int run();
int initWindow();
int initVulkan();
int createInstance();
int mainLoop();
int cleanup();
int checkValidationLayerSupport();
void setupDebugMessenger();
const char** getRequiredExtensions(uint32_t *extensionCount);
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData);
VkResult CreateDebugUtilsMessengerEXT(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
	const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger,
								   const VkAllocationCallbacks *pAllocator);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
void pickPhysicalDevice();
int isDeviceSuitable(VkPhysicalDevice device);

int triangle();


#endif
