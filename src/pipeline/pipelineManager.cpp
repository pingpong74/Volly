#include <memory>
#include <pipeline/pipelineManager.hpp>
#include "pipelineManagerImpl.hpp"

Volly::PipelineManager::PipelineManager(std::unique_ptr<PipelineManagerImpl> impl): impl(std::move(impl)) {}

Volly::PipelineManager::~PipelineManager() {}

Volly::ComputePipeline Volly::PipelineManager::createComputePipeline(const Volly::ComputePipelineCreateInfo&& computePipelineCreateInfo) {
    return impl->createComputePipeline(computePipelineCreateInfo);
}
