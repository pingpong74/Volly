#include <pipeline/pipeline.hpp>
#include <volk/volk.h>

Volly::ComputePipeline::ComputePipeline(VkDevice deivce, VkPipeline handle): device(deivce), handle(handle) {}
Volly::ComputePipeline::ComputePipeline(Volly::ComputePipeline&& other) noexcept : device(other.device), handle(other.handle) {}

Volly::ComputePipeline& Volly::ComputePipeline::operator=(Volly::ComputePipeline&& other) noexcept {
    device = other.device;
    handle = other.handle;

    other.handle = VK_NULL_HANDLE;
    return *this;
}

Volly::ComputePipeline::~ComputePipeline() {
    vkDestroyPipeline(device, handle, nullptr);
}
