#include "pipelineImpl.hpp"
#include <volk/volk.h>

Volly::ComputePipeline::ComputePipelineImpl::ComputePipelineImpl(VkDevice device, VkPipeline handle) : device(device), handle(handle) {}

Volly::ComputePipeline::ComputePipelineImpl::~ComputePipelineImpl() {
    vkDestroyPipeline(device, handle, nullptr);
}
