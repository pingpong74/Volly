#pragma once

#include "core/instance.hpp"
#include "core/swapchain.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace Volly {

    struct SwapchainCreateInfo {
        const char* name;
        VkSurfaceKHR surface;
        uint32_t width;
        uint32_t height;
    };

    class Device {
        public:
        class DeviceImpl;

        Device(std::unique_ptr<DeviceImpl> devImpl);
        Device(Device&&) noexcept;
        Device(const Device&) = delete;

        Device& operator=(Device&& other) noexcept;

        ~Device();

        Swapchain createSwapchain(const SwapchainCreateInfo&& swapchainCreateInfo);

        private:

        std::unique_ptr<DeviceImpl> impl;
    };
}
