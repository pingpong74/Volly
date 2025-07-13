#pragma once

#include "core/swapchain.hpp"
#include <volk/volk.h>

namespace Volly {
    class Swapchain::SwapchainImpl {
        public:

        SwapchainImpl(VkSwapchainKHR swapchain, VkDevice device);
        ~SwapchainImpl();

        private:

        VkSwapchainKHR handle;
        VkDevice device;
    };
}
