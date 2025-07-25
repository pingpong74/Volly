#pragma once

#include "pipeline/pipelineManager.hpp"
#include "resources/gpuResources.hpp"
#include "voxyConfig.hpp"
#include <core/instance.hpp>
#include <core/swapchain.hpp>
#include <memory>
#include <vma/vk_mem_alloc.h>

namespace Volly {

    struct SwapchainCreateInfo {
        const char* name = nullptr;
        VkSurfaceKHR surface;
        uint32_t width = initialWidth;
        uint32_t height = initialHeight;
    };

    struct BufferCreateFlags {
        VkBufferCreateFlags flags = 0;
    };

    struct BufferUsageFlags {
        VkBufferUsageFlags flags;

        BufferUsageFlags operator|(const BufferUsageFlags& other) const noexcept;
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
        AllocationCreateFlags operator|(const AllocationCreateFlags& other) const noexcept;
    };

    static constexpr AllocationCreateFlags createMapped = {VMA_ALLOCATION_CREATE_MAPPED_BIT};
    static constexpr AllocationCreateFlags sequenctialWriteBit = {VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT};
    static constexpr AllocationCreateFlags randomAccessBit = {VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT};
    static constexpr AllocationCreateFlags createDedicated = {VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT};
    static constexpr AllocationCreateFlags createNeverAllocated = {VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT};

    struct BufferCreateInfo {
        const char* name = nullptr;
        VkDeviceSize size;
        BufferCreateFlags createFlags = { 0 };
        BufferUsageFlags usageFlags;
        MemoryUsage memoryUsage;
        AllocationCreateFlags allocationFlags;
    };

    struct ImageCreateInfo {
        const char* name = nullptr;
        uint32_t width;
        uint32_t height;
    };

    struct PipelineManagerCreateInfo {
        const char* name = nullptr;
        const char* shaderDirectory;
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
        ImageID createImage(const ImageCreateInfo&& imageCreateInfo);

        PipelineManager createPipelineManager(const PipelineManagerCreateInfo&& pipelineManagerCreateInfo);

        private:

        std::unique_ptr<DeviceImpl> impl;
    };
}
