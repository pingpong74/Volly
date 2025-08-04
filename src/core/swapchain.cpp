#include <core/swapchain.hpp>

Volly::Swapchain::Swapchain(VkSwapchainKHR swapchain, VkDevice device): handle(swapchain), device(device) {}

Volly::Swapchain::Swapchain(Volly::Swapchain&& other) noexcept {}
Volly::Swapchain& Volly::Swapchain::operator=(Swapchain&& other) noexcept {
    return *this;
}

Volly::Swapchain::~Swapchain() {
    vkDestroySwapchainKHR(device, handle, nullptr);
}
