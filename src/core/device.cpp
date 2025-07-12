#include <core/device.hpp>
#include "deviceImpl.hpp"
#include <memory>

Volly::Device::Device(std::unique_ptr<Device::DeviceImpl> impl): impl(std::move(impl)) {}
Volly::Device::~Device() {}
