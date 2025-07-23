#include <memory>
#include "gpuResourcesImpl.hpp"
#include <resources/gpuResources.hpp>

Volly::BufferID::BufferID(std::shared_ptr<Buffer> impl) : impl(impl) {}
