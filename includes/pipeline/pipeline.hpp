#pragma once

#include <memory>
namespace Volly {

    class GraphicsPipeline {
        public:

        class GraphicsPipelineImpl;
    };

    class ComputePipeline {
        public:

        class ComputePipelineImpl;

        ComputePipeline(std::unique_ptr<ComputePipelineImpl> devImpl);
        ComputePipeline(ComputePipeline&&) noexcept;
        ComputePipeline(const ComputePipeline&) = delete;

        ComputePipeline& operator=(ComputePipeline&& other) noexcept;

        ~ComputePipeline();

        private:

        std::unique_ptr<ComputePipelineImpl> impl;
    };
}
