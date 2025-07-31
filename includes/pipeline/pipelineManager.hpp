#pragma once

#include <memory>
#include <volk/volk.h>
#include "pipeline.hpp"

namespace Volly {
    struct VetexInputRate {
        VkVertexInputRate rate = VK_VERTEX_INPUT_RATE_VERTEX;

        static VetexInputRate r;
    };

    struct GraphicsPipelineCreateInfo {
        const char* vertexShader;
        const char* fragmentShader;
        VetexInputRate vertexInputRate = {};
    };

    struct ComputePipelineCreateInfo {
        const char* computeShader;
    };

    class PipelineManager {
        public:

        class PipelineManagerImpl;

        PipelineManager(std::unique_ptr<PipelineManagerImpl> impl);
        ~PipelineManager();

        GraphicsPipeline createGraphicsPipeline(const GraphicsPipelineCreateInfo&& graphicsPipelineCreateInfo);
        ComputePipeline createComputePipeline(const ComputePipelineCreateInfo&& computePipelineCreateInfo);

        private:

        std::unique_ptr<PipelineManagerImpl> impl;
    };
}
