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

        static BufferUsageFlags transferSrc;
        static BufferUsageFlags transferDst;
        static BufferUsageFlags storageBuffer;
        static BufferUsageFlags indexBuffer;
        static BufferUsageFlags vertexBuffer;
        static BufferUsageFlags shaderDeviceAddress;
    };

    struct MemoryUsage {
        VmaMemoryUsage usage;

        static MemoryUsage preferAuto;
        static MemoryUsage preferDevice;
        static MemoryUsage preferHost;
    };

    struct AllocationCreateFlags {
        VmaAllocationCreateFlags flags;
        AllocationCreateFlags operator|(const AllocationCreateFlags& other) const noexcept;

        static AllocationCreateFlags createMapped;
        static AllocationCreateFlags sequenctialWriteBit;
        static AllocationCreateFlags randomAccessBit;
        static AllocationCreateFlags createDedicated;
        static AllocationCreateFlags createNeverAllocated;
    };

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
