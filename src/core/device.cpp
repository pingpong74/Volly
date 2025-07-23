#include <core/device.hpp>
#include <core/swapchain.hpp>
#include "deviceImpl.hpp"
#include <memory>

Volly::BufferUsageFlags Volly::BufferUsageFlags::operator|(const Volly::BufferUsageFlags& other) const noexcept  {
    return {flags | other.flags};
}

Volly::AllocationCreateFlags Volly::AllocationCreateFlags::operator|(const Volly::AllocationCreateFlags& other) const noexcept {
    return {flags | other.flags};
}

Volly::Device::Device(std::unique_ptr<Volly::Device::DeviceImpl> impl): impl(std::move(impl)) {}
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
