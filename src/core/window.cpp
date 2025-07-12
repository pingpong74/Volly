#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <core/window.hpp>
#include <vulkan/vulkan_core.h>

Volly::Window::Window(const char* windowName) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    windowPtr = glfwCreateWindow( 800, 600, windowName, nullptr, nullptr);

    glfwSetWindowUserPointer(windowPtr, this);
}

Volly::Window::~Window() {
    glfwDestroyWindow(windowPtr);
    glfwTerminate();
}

void Volly::Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(windowPtr);
}


std::vector<const char*> Volly::Window::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return extensions;
}

VkSurfaceKHR Volly::Window::createSurface(VkInstance instance) {
    VkSurfaceKHR surface;
    glfwCreateWindowSurface(instance, windowPtr, nullptr, &surface);
    return surface;
}
