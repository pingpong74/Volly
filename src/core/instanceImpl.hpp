#pragma once

#include "core/device.hpp"
#include <core/instance.hpp>
#include <volk/volk.h>
#include <voxyConfig.hpp>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace Volly {

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

    class Instance::InstanceImpl {
        public:
        InstanceImpl(const InstanceCreateInfo& instanceCreateInfo);
        ~InstanceImpl();

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
