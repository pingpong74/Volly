#include "gpuResourcesImpl.hpp"
#include <voxyConfig.hpp>

Volly::Buffer::Buffer(VkBufferCreateInfo& bufferCreateInfo, VmaAllocationCreateInfo& allocInfo, VmaAllocator allocator) : allocator(allocator) {
    VK_CHECK(vmaCreateBuffer(allocator, &bufferCreateInfo, &allocInfo, &handle, &bufferAllocation, &bufferAllocationInfo), "Failed to create buffer")
}

Volly::Buffer::~Buffer() {
    vmaDestroyBuffer(allocator, handle, bufferAllocation);
}
