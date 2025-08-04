#pragma once

#include <core/window.hpp>
#include <volk/volk.h>

#include <optional>
#include <vulkan/vulkan_core.h>

namespace Volly {
    struct InstanceCreateInfo {
        const char* name;
        Window& window;
        uint32_t apiVersion = VK_API_VERSION_1_3;
        #ifdef DEBUG
            bool enableValidationLayers = true;
        #else
            bool enableValidationLayers = false;
        #endif
    };

    struct DeviceCreateInfo {
        const char* name;
        bool raytracing = false;
        bool meshtracing = false;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentationFamily;
        std::optional<uint32_t> transferFamily;
        std::optional<uint32_t> computeFamily;

        inline bool isComplete() {
            return graphicsFamily.has_value() && presentationFamily.has_value() && transferFamily.has_value() && computeFamily.has_value();
        }

        static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presetMode;

        static SwapChainSupportDetails querySwapchainSupoort(VkPhysicalDevice dev, VkSurfaceKHR surface);
    };

    struct PhysicalDevice {
        VkPhysicalDevice handle;
        SwapChainSupportDetails swapchainSupportDetails;
        QueueFamilyIndices queueFamilIndices;
    };

    class Device;

    class Instance {
        public:
        Instance(const InstanceCreateInfo& instanceCreateInfo);
        ~Instance();

        Device createDevice(const DeviceCreateInfo& deviceCreateInfo);
        inline VkSurfaceKHR getSurface() { return surface; }

        private:

        VkInstance handle;
        VkSurfaceKHR surface;
        VkDebugUtilsMessengerEXT debugMessenger;
        bool validationLayersEnabled = false;

        void setupDebugMessenger();
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        PhysicalDevice selectPhysicalDevice();
    };
}
