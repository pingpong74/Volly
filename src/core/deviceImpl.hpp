#pragma once

#include "instanceImpl.hpp"
#include <volk/volk.h>
#include <vulkan/vulkan_core.h>

namespace Volly {
    class Device::DeviceImpl {
        public:

        DeviceImpl(VkDeviceCreateInfo deviceCreateInfo, PhysicalDevice physicalDevice);
        ~DeviceImpl();

        VkDevice handle;
        PhysicalDevice physicalDevice;
    };
}
