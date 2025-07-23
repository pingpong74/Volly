#pragma once

#include <core/device.hpp>
#include <resources/gpuResources.hpp>
#include <volk/volk.h>

namespace Volly {

    class Buffer {
        public:

        Buffer(VkBufferCreateInfo& bufferCreateInfo, VmaAllocationCreateInfo& allocInfo, VmaAllocator allocator);
        ~Buffer();

        private:

        VkBuffer handle = VK_NULL_HANDLE;
        VmaAllocation bufferAllocation = VK_NULL_HANDLE;
        VmaAllocationInfo bufferAllocationInfo = {};
        VmaAllocator allocator;
    };

    struct Image {

    };

    struct Blas {

    };

    struct Tlas {

    };
}
