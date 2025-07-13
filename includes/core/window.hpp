#pragma once

#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <volk/volk.h>
#include <vector>

namespace Volly {
    class Window {
        public:

        Window(const char* windowName);
        ~Window();

        void update();
        inline bool shouldClose() { return glfwWindowShouldClose(windowPtr); }

        std::vector<const char*> getRequiredExtensions();
        VkSurfaceKHR createSurface(VkInstance instance);

        private:

        GLFWwindow* windowPtr;

        uint32_t width;
        uint32_t height;

        bool isMinimized = false;
        bool swapchainOutOfDate = false;
    };
}
