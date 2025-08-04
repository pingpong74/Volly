#pragma once

#include <pipeline/pipelineManager.hpp>
#include <gpuResources.hpp>
#include <syncResources.hpp>
#include <core/instance.hpp>
#include <core/swapchain.hpp>
#include <vma/vk_mem_alloc.h>
#include <volk/volk.h>

namespace Volly {

    struct SwapchainCreateInfo {
        const char* name = nullptr;
        VkSurfaceKHR surface;
        uint32_t width = 800;
        uint32_t height = 600;
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

        Device() = delete;
        ~Device();

        Device (const Device& other) = delete;
        Device (const Device&& other);

        Swapchain createSwapchain(const SwapchainCreateInfo& swapchainCreateInfo);

        BufferID createBuffer(const BufferCreateInfo& bufferCreateInfo);
        ImageID createImage(const ImageCreateInfo& imageCreateInfo);

        PipelineManager createPipelineManager(const PipelineManagerCreateInfo& pipelineManagerCreateInfo);

        private:

        VkDevice handle = VK_NULL_HANDLE;
        PhysicalDevice physicalDevice;
        VmaAllocator vmaAllocator = VK_NULL_HANDLE;

        friend class Instance;
        //Constructor for the instance class only
        Device(VkInstance instance, VkDevice device, PhysicalDevice physicalDevice);
    };
}
