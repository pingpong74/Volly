#pragma once

#include <cstdint>
#include <memory>
#include <volk/volk.h>
#include <spvReflect/spirvReflect.h>

#include <unordered_set>
#include <vector>
#include <string>
#include "pipeline.hpp"

namespace Volly {
    struct VetexInputRate {
        VkVertexInputRate rate = VK_VERTEX_INPUT_RATE_VERTEX;
    };

    struct GraphicsPipelineCreateInfo {
        const char* vertexShader;
        const char* fragmentShader;
        VetexInputRate vertexInputRate = {};
    };

    struct ComputePipelineCreateInfo {
        const char* computeShader;
    };

    //Structs required for managing descriptors created
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
        const char* shader;
    };

    struct PipelineLayout {
        VkPipelineLayout layout;
        std::vector<std::string> shaders;
    };

    class PipelineManager {
        public:
        ~PipelineManager();

        GraphicsPipeline createGraphicsPipeline(const GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo);
        ComputePipeline createComputePipeline(const ComputePipelineCreateInfo& computePipelineCreateInfo);

        void writeToDescriptorSet(uint32_t setIndex, uint32_t binding);

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

        friend class Device;
        //Constructor for device class
        PipelineManager(VkPipelineCache cache, VkDevice device, std::string shaderDirectory);
    };
}
