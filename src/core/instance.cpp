#include <core/instance.hpp>
#include <memory>
#include <utility>


#include "core/device.hpp"
#include "instanceImpl.hpp"

Volly::Instance::Instance(std::unique_ptr<Volly::Instance::InstanceImpl> impl): impl(std::move(impl)) {}
Volly::Instance::Instance(Volly::Instance&& other) noexcept : impl(std::move(other.impl)) {}
Volly::Instance::~Instance() {}


Volly::Instance& Volly::Instance::operator=(Instance&& other) noexcept {
    impl = std::move(other.impl);
    return *this;
}

Volly::Instance Volly::Instance::create(const Volly::InstanceCreateInfo&& instanceCreateInfo) {
    std::unique_ptr<Volly::Instance::InstanceImpl> instanceImpl = std::make_unique<Volly::Instance::InstanceImpl>(instanceCreateInfo);
    return Instance(std::move(instanceImpl));
}

Volly::Device Volly::Instance::createDevice(const Volly::DeviceCreateInfo&& deviceCreateInfo) {
    return impl->createDevice(deviceCreateInfo);
}
