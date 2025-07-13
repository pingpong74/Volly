#pragma once

#include "core/device.hpp"
#include "core/swapchain.hpp"
#include "instanceImpl.hpp"
#include <volk/volk.h>
#include <vulkan/vulkan_core.h>

namespace Volly {
    class Device::DeviceImpl {
        public:

        DeviceImpl(VkDeviceCreateInfo deviceCreateInfo, PhysicalDevice physicalDevice);
        ~DeviceImpl();

        Swapchain createSwapchain(const SwapchainCreateInfo& createInfo);

        private:

        VkDevice handle;
        PhysicalDevice physicalDevice;
    };
}
