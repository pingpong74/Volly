#pragma once

#include <core/window.hpp>
#include <volk/volk.h>

#include <memory>
#include <vulkan/vulkan_core.h>

namespace Volly {
    struct InstanceCreateInfo {
        const char* name;
        Window& window;
        uint32_t apiVersion = VK_API_VERSION_1_3;
        bool enableValidationLayers = true;
    };

    struct DeviceCreateInfo {
        const char* name;
        bool raytracing = false;
        bool meshtracing = false;
    };

    class Device;

    class Instance {
        public:

        class InstanceImpl;

        static Instance create(const InstanceCreateInfo&& instanceCreateInfo);
        Instance(Instance&&) noexcept;
        Instance(const Instance&) = delete;

        Instance& operator=(Instance&& other) noexcept;

        ~Instance();

        Device createDevice(const DeviceCreateInfo&& deviceCreateInfo);
        VkSurfaceKHR getSurface();

        private:

        Instance(std::unique_ptr<InstanceImpl>);
        std::unique_ptr<InstanceImpl> impl;
    };
}
