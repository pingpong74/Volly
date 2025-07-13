#pragma once

#include <memory>
namespace Volly {
    class Swapchain {
        public:

        class SwapchainImpl;

        Swapchain(std::unique_ptr<SwapchainImpl> impl);
        Swapchain(Swapchain&&) noexcept;
        Swapchain(const Swapchain&) = delete;

        Swapchain& operator=(Swapchain&& other) noexcept;

        ~Swapchain();

        private:

        std::unique_ptr<SwapchainImpl> impl;

    };
}
