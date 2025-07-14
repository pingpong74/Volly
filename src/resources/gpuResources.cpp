#include <resources/gpuResources.hpp>

Volly::GpuResourceID::GpuResourceID(const Volly::GpuResourceID& other) noexcept: index(other.index.load()), reuseCount(other.reuseCount.load()) {}

Volly::GpuResourceID& Volly::GpuResourceID::operator=(const Volly::GpuResourceID& other) noexcept {
    index.store(other.index.load());
    reuseCount.store(other.reuseCount.load());
    return *this;
}
