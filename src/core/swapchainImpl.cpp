#include "swapchainImpl.hpp"
#include "core/swapchain.hpp"

Volly::Swapchain::SwapchainImpl::SwapchainImpl(VkSwapchainKHR swapchain, VkDevice device): handle(swapchain), device(device){}

Volly::Swapchain::SwapchainImpl::~SwapchainImpl() {
    vkDestroySwapchainKHR(device, handle, nullptr);
}
