#include <core/device.hpp>
#include <core/swapchain.hpp>
#include "deviceImpl.hpp"
#include <memory>


Volly::BufferUsageFlags Volly::BufferUsageFlags::transferSrc = {VK_BUFFER_USAGE_TRANSFER_SRC_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::transferDst = {VK_BUFFER_USAGE_TRANSFER_DST_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::storageBuffer = {VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::indexBuffer = {VK_BUFFER_USAGE_INDEX_BUFFER_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::vertexBuffer = {VK_BUFFER_USAGE_VERTEX_BUFFER_BIT};
Volly::BufferUsageFlags Volly::BufferUsageFlags::shaderDeviceAddress = {VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT};

Volly::MemoryUsage Volly::MemoryUsage::preferAuto = {VMA_MEMORY_USAGE_AUTO};
Volly::MemoryUsage Volly::MemoryUsage::preferDevice = {VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE};
Volly::MemoryUsage Volly::MemoryUsage::preferHost = {VMA_MEMORY_USAGE_AUTO_PREFER_HOST};

Volly::AllocationCreateFlags Volly::AllocationCreateFlags::createMapped = {VMA_ALLOCATION_CREATE_MAPPED_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::sequenctialWriteBit = {VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::randomAccessBit = {VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::createDedicated = {VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT};
Volly::AllocationCreateFlags Volly::AllocationCreateFlags::createNeverAllocated = {VMA_ALLOCATION_CREATE_NEVER_ALLOCATE_BIT};

Volly::BufferUsageFlags Volly::BufferUsageFlags::operator|(const Volly::BufferUsageFlags& other) const noexcept  {
    return {flags | other.flags};
}

Volly::AllocationCreateFlags Volly::AllocationCreateFlags::operator|(const Volly::AllocationCreateFlags& other) const noexcept {
    return {flags | other.flags};
}

Volly::Device::Device(std::unique_ptr<DeviceImpl> impl): impl(std::move(impl)) {}
Volly::Device::Device(Device&& other) noexcept : impl(std::move(other.impl)) {}

Volly::Device& Volly::Device::operator=(Device&& other) noexcept {
    impl = std::move(other.impl);
    return *this;
}

Volly::Device::~Device() {}

Volly::Swapchain Volly::Device::createSwapchain(const SwapchainCreateInfo&& swapchainCreateInfo) {
    return impl->createSwapchain(swapchainCreateInfo);
}

Volly::BufferID Volly::Device::createBuffer(const BufferCreateInfo&& createInfo) {
    return impl->createBuffer(createInfo);
}

Volly::ImageID Volly::Device::createImage(const ImageCreateInfo&& createInfo) {
    return impl->createImage(createInfo);
}

Volly::PipelineManager Volly::Device::createPipelineManager(const PipelineManagerCreateInfo&& createInfo) {
    return impl->createPipelineManager(createInfo);
}
