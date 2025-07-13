#include "deviceImpl.hpp"
#include "core/device.hpp"
#include "core/swapchain.hpp"
#include "instanceImpl.hpp"
#include "swapchainImpl.hpp"
#include <limits>
#include <algorithm>
#include <memory>

Volly::Device::DeviceImpl::DeviceImpl(VkDeviceCreateInfo deviceCreateInfo, PhysicalDevice physicalDevice): physicalDevice(physicalDevice) {
    VK_CHECK(vkCreateDevice(physicalDevice.handle, &deviceCreateInfo, nullptr, &handle), "Failed to create logical device")
}

Volly::Device::DeviceImpl::~DeviceImpl() {
    vkDestroyDevice(handle, nullptr);
}

Volly::Swapchain Volly::Device::DeviceImpl::createSwapchain(const SwapchainCreateInfo& swapchainCreateInfo) {
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

	VkSwapchainCreateInfoKHR createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.imageFormat = swapchainFormat.format;
	createInfo.imageColorSpace = swapchainFormat.colorSpace;
	createInfo.imageExtent = swapchainExtent;
	createInfo.presentMode = swapchainPresentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	createInfo.surface = swapchainCreateInfo.surface;
	createInfo.minImageCount = imageCount;

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

	createInfo.preTransform = physicalDevice.swapchainSupportDetails.capabilities.currentTransform;

	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkSwapchainKHR swapchain;
	VK_CHECK(vkCreateSwapchainKHR(handle, &createInfo, nullptr, &swapchain), "Failed to create swapchain")

	std::unique_ptr<Volly::Swapchain::SwapchainImpl> swapchainImpl = std::make_unique<Volly::Swapchain::SwapchainImpl>(swapchain, handle);

	return Swapchain(std::move(swapchainImpl));
}
