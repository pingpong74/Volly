#include <pipeline/pipeline.hpp>
#include "pipelineImpl.hpp"

Volly::ComputePipeline::ComputePipeline(std::unique_ptr<ComputePipeline::ComputePipelineImpl> impl): impl(std::move(impl)) {}
Volly::ComputePipeline::ComputePipeline(Volly::ComputePipeline&& other) noexcept : impl(std::move(other.impl)) {}

Volly::ComputePipeline& Volly::ComputePipeline::operator=(Volly::ComputePipeline&& other) noexcept {
    impl = std::move(other.impl);
    return *this;
}

Volly::ComputePipeline::~ComputePipeline() {}
