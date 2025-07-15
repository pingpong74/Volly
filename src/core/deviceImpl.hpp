#pragma once

#include <core/device.hpp>
#include <core/swapchain.hpp>
#include "instanceImpl.hpp"
#include "../resources/gpuResourcesImpl.hpp"

#include <deque>
#include <volk/volk.h>
#include <vma/vk_mem_alloc.h>
#include <queue>

namespace Volly {
    class Device::DeviceImpl {
        public:

        DeviceImpl(VkInstance instance, VkDeviceCreateInfo deviceCreateInfo, PhysicalDevice physicalDevice);
        ~DeviceImpl();

        Swapchain createSwapchain(const SwapchainCreateInfo& createInfo);
        BufferID createBuffer(const BufferCreateInfo& createInfo);

        private:

        VkDevice handle;
        PhysicalDevice physicalDevice;
        VmaAllocator vmaAllocator;

        ResourcePool<Buffer> bufferPool;

        std::deque<Buffer> delBufferQueue;
    };
}
