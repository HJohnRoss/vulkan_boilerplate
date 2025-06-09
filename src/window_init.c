#include "window_init.h"

GLFWwindow *window;
VkInstance instance;

const char *validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const size_t validationLayerCount =
    sizeof(validationLayers) / sizeof(validationLayers[0]);
VkDebugUtilsMessengerEXT debugMessenger;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

int run() {
    if (initWindow() == -1) {
        printf("Window Initiation function failed\n");
        return -1;
    }
    if (initVulkan() == -1) {
        printf("Vulkan Initiation function failed\n");
        return -1;
    }
    printf("Vulkan initiated successfully\n");
    if (mainLoop() == -1) {
        printf("Main loop function failed\n");
        return -1;
    }
    printf("Main loop conpleted successfully\n");
    if (cleanup() == -1) {
        printf("cleanup function failed\n");
        return -1;
    }
    printf("cleanup completed successfully\n");
    return 0;
}

int initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    return 0;
}

int initVulkan() {
    if (createInstance() != 0) {
        return -1;
    }
    setupDebugMessenger();
    pickPhysicalDevice();

    return 0;
}

int createInstance() {
    if (ENABLE_VALIDATION_LAYERS && checkValidationLayerSupport() != 0) {
        fprintf(stderr, "validation layers are requested, but not available\n");
        return -1;
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extensionCount = 0;
    const char **extensions = getRequiredExtensions(&extensionCount);
    if (!extensions) {
        return -1;
    }
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;

        populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = NULL;
    }

    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance!\n");
        free(extensions);
        return -1;
    }
    free(extensions);
    return 0;
}

int mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    return 0;
}

int cleanup() {
    if (ENABLE_VALIDATION_LAYERS) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
        debugMessenger = NULL;
    }

    vkDestroyInstance(instance, NULL);
    instance = NULL;

    glfwDestroyWindow(window);
    window = NULL;

    glfwTerminate();
    return 0;
}

int checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties *availableLayers =
        malloc(layerCount * sizeof(VkLayerProperties));
    if (!availableLayers) {
        fprintf(stderr, "Failed to allocate memory for availableLayers\n");
        return -1;
    }

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (size_t i = 0; i < validationLayerCount; i++) {
        const char *layerName = validationLayers[i];
        int layerFound = 0;

        for (size_t j = 0; j < layerCount; j++) {
            if (strcmp(layerName, availableLayers[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }

        if (!layerFound) {
            fprintf(stderr, "Validation layer not found: %s\n", layerName);
            free(availableLayers);
            return -1;
        }
    }

    free(availableLayers);
    return 0;
}

const char **getRequiredExtensions(uint32_t *extensionCount) {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions =
        glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (!glfwExtensions) {
        fprintf(stderr, "Failed to get GLFW extensions");
        return NULL;
    }

    uint32_t totalExtensionCount = glfwExtensionCount;
    if (ENABLE_VALIDATION_LAYERS) {
        totalExtensionCount++;
    }

    const char **extensions =
        malloc(totalExtensionCount * sizeof(const char *));
    if (!extensions) {
        fprintf(stderr, "Failed to allocate memory for extensions\n");
        return NULL;
    }

    memcpy(extensions, glfwExtensions,
           glfwExtensionCount * sizeof(const char *));

    if (ENABLE_VALIDATION_LAYERS) {
        extensions[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    *extensionCount = totalExtensionCount;

    return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {

    fprintf(stderr, "validation layer: %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {

    PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

void setupDebugMessenger() {
    if (!ENABLE_VALIDATION_LAYERS) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, NULL,
                                     &debugMessenger) != VK_SUCCESS) {
        fprintf(stderr, "Failed to setup debug messenger\n");
    }
}

void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT *createInfo) {
    // Zero out the structure
    memset(createInfo, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

    // Set the structure type
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    // Set the message severity flags
    createInfo->messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    // Set the message type flags
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    // Set the callback function
    createInfo->pfnUserCallback = debugCallback;
}

void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support\n");
        return;
    }

    VkPhysicalDevice *devices =
        (VkPhysicalDevice *)malloc(deviceCount * sizeof(VkPhysicalDevice));
    if (!devices) {
        fprintf(stderr, "Failed to allocate memory for devices\n");
        return;
    }

    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (uint32_t i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(devices[i])) {
            physicalDevice = devices[i];
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        fprintf(stderr, "Failed to find a suitable GPU\n");
    }
}

int isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Get the list of extensions supported by the device
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

    // Allocate memory for the list of extensions
    VkExtensionProperties *availableExtensions =
        malloc(extensionCount * sizeof(VkExtensionProperties));
    if (!availableExtensions) {
        fprintf(stderr, "Failed to allocate memory for available extensions\n");
        return 0;
    }

    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount,
                                         availableExtensions);

    // Check if required extensions are available
    int foundSwapChainExtension = 0;
    for (uint32_t i = 0; i < extensionCount; i++) {
        if (strcmp(availableExtensions[i].extensionName,
                   VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
            foundSwapChainExtension = 1;
            break;
        }
    }

    free(availableExtensions); // Free the memory

    if (!foundSwapChainExtension) {
        return 0; // Swapchain extension is required
    }

    // Check for a suitable graphics queue
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    VkQueueFamilyProperties *queueFamilies =
        malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    if (!queueFamilies) {
        fprintf(stderr, "Failed to allocate memory for queue families\n");
        return 0;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies);

    int hasGraphicsQueue = 0;
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            hasGraphicsQueue = 1;
            break;
        }
    }

    free(queueFamilies); // Free the memory

    return hasGraphicsQueue; // Only return true if there's a suitable graphics
                             // queue
}
