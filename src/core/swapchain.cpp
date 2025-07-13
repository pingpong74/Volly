#include <core/swapchain.hpp>
#include "swapchainImpl.hpp"

Volly::Swapchain::Swapchain(std::unique_ptr<Volly::Swapchain::SwapchainImpl> impl): impl(std::move(impl)) {}
Volly::Swapchain::Swapchain(Volly::Swapchain&& other) noexcept : impl(std::move(other.impl)) {}

Volly::Swapchain& Volly::Swapchain::operator=(Swapchain&& other) noexcept {
    impl = std::move(other.impl);
    return *this;
}

Volly::Swapchain::~Swapchain() {}
