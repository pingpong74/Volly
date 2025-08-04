#include <core/instance.hpp>
#include <core/device.hpp>
#include "../vollyConfig.hpp"

#include <iostream>
#include <set>

Volly::Instance::Instance(const Volly::InstanceCreateInfo& instanceCreateInfo) {
    VK_CHECK(volkInitialize(), "Failed to initialize volk")

	//Get all required extensions
	std::vector<const char*> requiredExtensions = instanceCreateInfo.window.getRequiredExtensions();
	if(instanceCreateInfo.enableValidationLayers) requiredExtensions.push_back("VK_EXT_debug_utils");

	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = instanceCreateInfo.name;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.apiVersion = instanceCreateInfo.apiVersion;

	//get the extensions
	VkInstanceCreateInfo vkInstanceCreateInfo{};
	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pApplicationInfo = &app_info;
	vkInstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	vkInstanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
	vkInstanceCreateInfo.enabledLayerCount = 0;

	//check for validation layers and put them in the strust if enabled
	if(instanceCreateInfo.enableValidationLayers) {
	    //if(!checkValidationLayerSupport()) throw std::runtime_error("Validation layers requested but not available");
		vkInstanceCreateInfo.enabledLayerCount = validationLayersCount;
		vkInstanceCreateInfo.ppEnabledLayerNames = validationLayers;
		validationLayersEnabled = true;
	}

	VK_CHECK(vkCreateInstance(&vkInstanceCreateInfo, nullptr, &handle), "Failed to create instance")
	surface = instanceCreateInfo.window.createSurface(handle);

	volkLoadInstance(handle);
	if(validationLayersEnabled) setupDebugMessenger();
}

Volly::Instance::Instance::~Instance() {
    if(validationLayersEnabled) vkDestroyDebugUtilsMessengerEXT(handle, debugMessenger, nullptr);
    vkDestroySurfaceKHR(handle, surface, nullptr);
    vkDestroyInstance(handle, nullptr);
}

void Volly::Instance::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;

	VK_CHECK(vkCreateDebugUtilsMessengerEXT(handle, &createInfo, nullptr, &debugMessenger), "Failed to create debug messenger")
}

VKAPI_ATTR VkBool32 VKAPI_CALL Volly::Instance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
	    std::cerr << "validationlayer:" << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

Volly::PhysicalDevice Volly::Instance::selectPhysicalDevice() {
    uint32_t deviceCount;
    VK_CHECK(vkEnumeratePhysicalDevices(handle, &deviceCount, nullptr), "Failed to enumerate device");

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    VK_CHECK(vkEnumeratePhysicalDevices(handle, &deviceCount, physicalDevices.data()), "Failed to enumerate device");

    PhysicalDevice bestPhysicalDevice{};

    int max = 0;

    for(const auto& physicalDevice: physicalDevices) {
        int score = 0;

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        QueueFamilyIndices queueFamilyIndex = QueueFamilyIndices::findQueueFamilies(physicalDevice, surface);
        SwapChainSupportDetails details = SwapChainSupportDetails::querySwapchainSupoort(physicalDevice, surface);

        if(details.presetMode.empty() || details.formats.empty() || !queueFamilyIndex.isComplete()) {
            continue;
        }
        else {
            score = 10;
        }

        if(score > max) {
            bestPhysicalDevice = {
                .handle = physicalDevice,
                .swapchainSupportDetails = details,
                .queueFamilIndices = queueFamilyIndex,
            };
            max = score;
        }
    }

    if(max == 0 ) throw std::runtime_error("Failed to find a suitable physical Device");

    return bestPhysicalDevice;
}

Volly::Device Volly::Instance::createDevice(const DeviceCreateInfo& deviceCreateInfo) {
    PhysicalDevice physicalDevice = selectPhysicalDevice();

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.handle, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice.handle, &queueFamilyCount, queueFamilyProperties.data());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {
	    physicalDevice.queueFamilIndices.graphicsFamily.value(),
		physicalDevice.queueFamilIndices.presentationFamily.value(),
		physicalDevice.queueFamilIndices.transferFamily.value(),
	    physicalDevice.queueFamilIndices.computeFamily.value()
	};
	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	//Needed for controlling buffer access
	VkPhysicalDeviceVulkan12Features features12 = {};
	features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	features12.shaderInt8 = VK_TRUE;
	features12.storageBuffer8BitAccess = VK_TRUE;
	features12.bufferDeviceAddress = VK_TRUE;
	features12.pNext = nullptr;

	// Root device features structure
	VkPhysicalDeviceFeatures2 deviceFeatures2{};
	deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	deviceFeatures2.features.shaderInt64 = VK_TRUE;
	deviceFeatures2.pNext = &features12;

	std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain"};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = VK_NULL_HANDLE;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.pNext = &deviceFeatures2;

	if(validationLayersEnabled) {
	    createInfo.enabledLayerCount = validationLayersCount;
        createInfo.ppEnabledLayerNames = validationLayers;
	}

	VkDevice device;
	VK_CHECK(vkCreateDevice(physicalDevice.handle, &createInfo, nullptr, &device), "Failed to create logical device")

    return Device(handle, device, physicalDevice);
}

Volly::QueueFamilyIndices Volly::QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    Volly::QueueFamilyIndices queueFamily;

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    int i = 0;

    for(const auto& properties : queueFamilyProperties) {
        VkBool32 presetationQueuePresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presetationQueuePresent);

        if(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) queueFamily.graphicsFamily = i;

        if(presetationQueuePresent) queueFamily.presentationFamily = i;

        if(properties.queueFlags & VK_QUEUE_TRANSFER_BIT && !(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) queueFamily.transferFamily = i;

        if(properties.queueFlags & VK_QUEUE_COMPUTE_BIT && !(properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) queueFamily.computeFamily = i;

        if(queueFamily.isComplete()) break;

        i++;

    }

    return queueFamily;
}

Volly::SwapChainSupportDetails Volly::SwapChainSupportDetails::querySwapchainSupoort(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    Volly::SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface,&formatCount, nullptr);

    if(formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, nullptr);

    if(presentCount != 0) {
        details.presetMode.resize(presentCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, details.presetMode.data());
    }

    return details;
}
