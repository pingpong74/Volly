#pragma once

#include "pipeline/pipeline.hpp"
#include <spvReflect/spirvReflect.h>
#include <pipeline/pipelineManager.hpp>

#include <vector>
#include <volk/volk.h>
#include <string>
#include <unordered_set>
#include <vulkan/vulkan_core.h>

namespace Volly {
    class PipelineManager::PipelineManagerImpl {
        public:
        PipelineManagerImpl(VkPipelineCache cache, VkDevice device, std::string shaderDirectory);
        ~PipelineManagerImpl();

        GraphicsPipeline createGraphicsPipeline(const GraphicsPipelineCreateInfo& createInfo);
        ComputePipeline createComputePipeline(const ComputePipelineCreateInfo& createInfo);

        struct DescriptorBindingInfo {
            uint32_t binding;
            VkDescriptorType type;
            uint32_t count;
            VkShaderStageFlags stageFlags;

            inline bool operator==(const DescriptorBindingInfo& other) const noexcept { return binding == other.binding && type == other.type && count == other.count; }
        };

        struct DescriptorSet {
            uint32_t set;
            std::vector<DescriptorBindingInfo> bindings;
            VkDescriptorSetLayout layout;
            std::vector<std::string> shaders;

            void pushBack(DescriptorBindingInfo binding);
        };

        struct VertexAttributeInfo {
            VkVertexInputAttributeDescription inputAttributeDescription;
            VkVertexInputBindingDescription bindingDescription;
            const std::string shader;
        };

        struct PipelineLayout {
            VkPipelineLayout layout;
            std::vector<std::string> shaders;
        };

        private:

        VkPipelineCache cache = VK_NULL_HANDLE;
        VkDevice device;

        std::string shaderDirectory;
        std::vector<DescriptorSet> sets;
        std::vector<VertexAttributeInfo> vertexAttributes;

        std::vector<PipelineLayout> pipelineLayouts;

        std::vector<uint32_t> readShader(const std::string& path);
        void getDescriptorSets(SpvReflectShaderModule& module, const std::string& path);
        void getVertexInput(SpvReflectShaderModule& module, const std::string& path);
        VkShaderModule createShaderModue(const std::string& path);
        PipelineLayout createPipelineLayout(std::unordered_set<std::string>&& shaders);
    };
}
