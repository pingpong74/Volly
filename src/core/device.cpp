#include <core/device.hpp>
#include <core/swapchain.hpp>
#include <pipeline/pipelineManager.hpp>
#include "../vollyConfig.hpp"


#include <limits>
#include <algorithm>
#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>


//Declaring buffer flags
Volly::BufferUsageFlags Volly::BufferUsageFlags::transferSrc = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::transferDst = {VK_BUFFER_USAGE_TRANSFER_DST_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::storageBuffer = {VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::indexBuffer = {VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::vertexBuffer = {VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::shaderDeviceAddress = {VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT};

Volly::MemoryUsage Volly::MemoryUsage::preferAuto = {VMA_MEMORY_USAGE_AUTO};
Volly::MemoryUsage Volly::MemoryUsage::preferDevice = {VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE};
Volly::MemoryUsage Volly::MemoryUsage::preferHost = {VMA_MEMORY_USAGE_AUTO_PREFER_HOST};

Volly::AllocationCreateFlags Volly::AllocationCreateFlags::createMapped = {VMA_ALLOCATION_CREATE_MAPPED_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::sequenctialWriteBit = {VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::randomAccessBit = {VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::createDedicated = {VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::createNeverAllocated = {VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT};

//Flag operations
Volly::BufferUsageFlags Volly::BufferUsageFlags::operator|(const Volly::BufferUsageFlags& other) const noexcept  {
    return {flags | other.flags};
}

Volly::AllocationCreateFlags Volly::AllocationCreateFlags::operator|(const Volly::AllocationCreateFlags& other) const noexcept {
    return {flags | other.flags};
}

//Start Logical device functions
Volly::Device::Device(VkInstance instance, VkDevice handle, Volly::PhysicalDevice physicalDevice): physicalDevice(physicalDevice), handle(handle) {
    //Load device level functions
    volkLoadDevice(handle);

    //Create vma alloator
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = physicalDevice.handle;
    allocatorCreateInfo.device = handle;
    allocatorCreateInfo.instance = instance;
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    VK_CHECK(vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, &vulkanFunctions), "Failed to load vma functions from volk");
    VK_CHECK(vmaCreateAllocator(&allocatorCreateInfo, &vmaAllocator), "Failed to create vma allocator");
}

Volly::Device::~Device() {
    vmaDestroyAllocator(vmaAllocator);
    vkDestroyDevice(handle, nullptr);
}

Volly::Swapchain Volly::Device::createSwapchain(const SwapchainCreateInfo& swapchainCreateInfo) {
    //Get the required format, preset mode and extent
    VkSurfaceFormatKHR swapchainFormat = [this]{
        for(const auto& format : physicalDevice.swapchainSupportDetails.formats) {
            if(format.format == VK_FORMAT_B8G8R8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) return format;
        }

        return physicalDevice.swapchainSupportDetails.formats[0];
    }();
	VkPresentModeKHR swapchainPresentMode = [this]{
	    for(const auto& presentationMode: physicalDevice.swapchainSupportDetails.presetMode) {
			if(presentationMode == VK_PRESENT_MODE_MAILBOX_KHR) return presentationMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}();
	VkExtent2D swapchainExtent = [this](uint32_t width, uint32_t height){
        if(physicalDevice.swapchainSupportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) return physicalDevice.swapchainSupportDetails.capabilities.currentExtent;

		VkExtent2D extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

		extent.width = std::clamp(extent.width, physicalDevice.swapchainSupportDetails.capabilities.minImageExtent.width, physicalDevice.swapchainSupportDetails.capabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, physicalDevice.swapchainSupportDetails.capabilities.minImageExtent.height, physicalDevice.swapchainSupportDetails.capabilities.maxImageExtent.height);

		return extent;
	}(swapchainCreateInfo.width, swapchainCreateInfo.height);

	uint32_t imageCount = physicalDevice.swapchainSupportDetails.capabilities.minImageCount;

	if(imageCount < physicalDevice.swapchainSupportDetails.capabilities.maxImageCount && physicalDevice.swapchainSupportDetails.capabilities.maxImageCount > 0) {
	    imageCount = physicalDevice.swapchainSupportDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {
	    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	    .pNext = nullptr,
	    .surface = swapchainCreateInfo.surface,
	    .minImageCount = imageCount,
	    .imageFormat = swapchainFormat.format,
	    .imageColorSpace = swapchainFormat.colorSpace,
	    .imageExtent = swapchainExtent,
	    .imageArrayLayers = 1,
	    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
	    .preTransform = physicalDevice.swapchainSupportDetails.capabilities.currentTransform,
	    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	    .presentMode = swapchainPresentMode,
	    .clipped = VK_TRUE,
	    .oldSwapchain = VK_NULL_HANDLE,
	};

	//Check if there are diffent graphics and presentation queues
	uint32_t queues[] = {physicalDevice.queueFamilIndices.graphicsFamily.value(), physicalDevice.queueFamilIndices.presentationFamily.value()};

	if(queues[0] != queues[1]) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queues;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
	}

	//Create the swapchain
	VkSwapchainKHR swapchain;
	VK_CHECK(vkCreateSwapchainKHR(handle, &createInfo, nullptr, &swapchain), "Failed to create swapchain named")

	return Swapchain(swapchain, handle);
}

Volly::BufferID Volly::Device::createBuffer(const BufferCreateInfo& createInfo) {
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = nullptr;
    bufferCreateInfo.flags   = createInfo.createFlags.flags,
    bufferCreateInfo.size = createInfo.size,
    bufferCreateInfo.usage = createInfo.usageFlags.flags,
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.flags = createInfo.allocationFlags.flags;
    allocInfo.usage = createInfo.memoryUsage.usage;
    allocInfo.requiredFlags = 0;
    allocInfo.preferredFlags = 0;
    allocInfo.pool = VK_NULL_HANDLE;
}

Volly::ImageID Volly::Device::createImage(const ImageCreateInfo& createInfo) {
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.extent = {createInfo.width, createInfo.height, 1};
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VmaAllocatorCreateInfo allocInfo{};
}

Volly::PipelineManager Volly::Device::createPipelineManager(const PipelineManagerCreateInfo& pipelineManagerCreateInfo) {
    VkPipelineCacheCreateInfo cacheCreateInfo = {};
    cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    VkPipelineCache cache = VK_NULL_HANDLE;
    //VK_CHECK(vkCreatePipelineCache(handle, &cacheCreateInfo, nullptr, &cache), "Failed to create pipeline cache")

    return PipelineManager(cache, handle, pipelineManagerCreateInfo.shaderDirectory);
}
