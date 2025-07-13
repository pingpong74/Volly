#pragma once

#include <core/device.hpp>
#include <core/swapchain.hpp>
#include "instanceImpl.hpp"
#include <resources/gpuResources.hpp>

#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>

namespace Volly {
    class Device::DeviceImpl {
        public:

        DeviceImpl(VkInstance instance, VkDeviceCreateInfo deviceCreateInfo, PhysicalDevice physicalDevice);
        ~DeviceImpl();

        Swapchain createSwapchain(const SwapchainCreateInfo& createInfo);

        private:

        VkDevice handle;
        PhysicalDevice physicalDevice;
        VmaAllocator vmaAllocator;
    };
}
