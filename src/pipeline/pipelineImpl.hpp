#pragma once

#include <pipeline/pipeline.hpp>
#include <volk/volk.h>

namespace Volly {
    class Pipeline {

    };

    class GraphicsPipeline::GraphicsPipelineImpl {

    };

    class ComputePipeline::ComputePipelineImpl {
        public:

        ComputePipelineImpl(VkDevice device, VkPipeline handle);
        ~ComputePipelineImpl();

        private:
        VkDevice device;
        VkPipeline handle;
    };
}
