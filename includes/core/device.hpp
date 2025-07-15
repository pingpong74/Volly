#pragma once

#include "resources/gpuResources.hpp"
#include <core/instance.hpp>
#include <core/swapchain.hpp>
#include <memory>
#include <vma/vk_mem_alloc.h>

namespace Volly {

    struct SwapchainCreateInfo {
        const char* name;
        VkSurfaceKHR surface;
        uint32_t width;
        uint32_t height;
    };

    struct BufferCreateFlags {
        VkBufferCreateFlags flags = 0;
    };

    struct BufferUsageFlags {
        VkBufferUsageFlags flags;

        BufferUsageFlags& operator|(BufferUsageFlags& other) noexcept;
    };

    static constexpr BufferUsageFlags transferSrc = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
    static constexpr BufferUsageFlags transferDst = {VK_BUFFER_USAGE_TRANSFER_DST_BIT};
    static constexpr BufferUsageFlags storageBuffer = {VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT};
    static constexpr BufferUsageFlags indexBuffer = {VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
    static constexpr BufferUsageFlags vertexBuffer = {VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
    static constexpr BufferUsageFlags shaderDeviceAddress = {VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT};

    struct MemoryUsage {
        VmaMemoryUsage usage;
    };

    static constexpr MemoryUsage preferAuto = {VMA_MEMORY_USAGE_AUTO};
    static constexpr MemoryUsage preferDevice = {VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE};
    static constexpr MemoryUsage preferHost = {VMA_MEMORY_USAGE_AUTO_PREFER_HOST};

    struct AllocationCreateFlags {
        VmaAllocationCreateFlags flags;
    };

    static constexpr AllocationCreateFlags createDedicated = {VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT};

    struct BufferCreateInfo {
        const char* name;
        VkDeviceSize size;
        BufferCreateFlags createFlags = { 0 };
        BufferUsageFlags usageFlags;
        MemoryUsage memoryUsage;
        AllocationCreateFlags allocationFlags;
    };

    class Device {
        public:
        class DeviceImpl;

        Device(std::unique_ptr<DeviceImpl> devImpl);
        Device(Device&&) noexcept;
        Device(const Device&) = delete;

        Device& operator=(Device&& other) noexcept;

        ~Device();

        Swapchain createSwapchain(const SwapchainCreateInfo&& swapchainCreateInfo);

        BufferID createBuffer(const BufferCreateInfo&& bufferCreateInfo);

        private:

        std::unique_ptr<DeviceImpl> impl;
    };
}
