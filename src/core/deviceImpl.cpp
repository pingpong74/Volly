#include "deviceImpl.hpp"
#include "core/device.hpp"
#include "instanceImpl.hpp"

Volly::Device::DeviceImpl::DeviceImpl(VkDeviceCreateInfo deviceCreateInfo, PhysicalDevice physicalDevice): physicalDevice(physicalDevice) {
    VK_CHECK(vkCreateDevice(physicalDevice.handle, &deviceCreateInfo, nullptr, &handle), "Failed to create logical device")
}

Volly::Device::DeviceImpl::~DeviceImpl() {
    vkDestroyDevice(handle, nullptr);
}
