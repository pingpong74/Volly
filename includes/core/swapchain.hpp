#pragma once

#include <volk/volk.h>

namespace Volly {
    class Device;

    class Swapchain {
        public:

        Swapchain() = delete;
        ~Swapchain();
        Swapchain(Swapchain&&) noexcept;
        Swapchain(const Swapchain&) = delete;

        Swapchain& operator=(Swapchain&& other) noexcept;

        private:

        VkSwapchainKHR handle = VK_NULL_HANDLE;
        VkDevice device;

        VkSurfaceFormatKHR format;
        VkPresentModeKHR presetMode;
        VkExtent2D extent;

        friend class Device;
        //Constructor for Device class to access
        Swapchain(VkSwapchainKHR swapchain, VkDevice device);
    };
}
