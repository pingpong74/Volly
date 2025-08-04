#pragma once

#include <volk/volk.h>

namespace Volly {

    class GraphicsPipeline {

    };

    class ComputePipeline {
        public:

        ComputePipeline(ComputePipeline&&) noexcept;
        ComputePipeline(const ComputePipeline&) = delete;

        ComputePipeline& operator=(ComputePipeline&& other) noexcept;

        ~ComputePipeline();

        private:

        VkDevice device;
        VkPipeline handle = VK_NULL_HANDLE;

        friend class PipelineManager;
        //Constructor for pipeline manager
        ComputePipeline(VkDevice device, VkPipeline handle);
    };
}
