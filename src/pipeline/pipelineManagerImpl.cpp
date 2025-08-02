#include "pipelineManagerImpl.hpp"
#include "pipeline/pipeline.hpp"
#include <pipeline/pipelineManager.hpp>
#include "pipelineImpl.hpp"
#include <spvReflect/spirvReflect.h>
#include "voxyConfig.hpp"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <vulkan/vulkan_core.h>

void Volly::PipelineManager::PipelineManagerImpl::DescriptorSet::pushBack(Volly::PipelineManager::PipelineManagerImpl::DescriptorBindingInfo binding) {
    for(size_t i = 0; i < bindings.size(); i++) {
        if(binding == bindings[i]) {
            bindings[i].stageFlags |= binding.stageFlags;
            return;
        }
    }

    bindings.push_back(binding);
}

Volly::PipelineManager::PipelineManagerImpl::PipelineManagerImpl(VkPipelineCache cache, VkDevice device, std::string shaderDirectory) : cache(cache), device(device), shaderDirectory(shaderDirectory) {
    //Iterate over all the shaders in the directory
    for(const auto& file: std::filesystem::directory_iterator(shaderDirectory)) {
        if(file.path().extension() == ".spv") {
            //Load the spv shader
            std::vector<uint32_t> buffer = readShader(file.path().string());

            //create a spv shader module
            SpvReflectShaderModule module;
            if(spvReflectCreateShaderModule(buffer.size() * sizeof(uint32_t), buffer.data(), &module) != SPV_REFLECT_RESULT_SUCCESS) {
                throw std::runtime_error("Failed to generate module from spv data");
            }

            //Extract descriptor set info
            getDescriptorSets(module, file.path().string());

            //If the shader is a vertex shader extract the vertex input data
            if(module.shader_stage == SPV_REFLECT_SHADER_STAGE_VERTEX_BIT) getVertexInput(module, file.path().string());

            spvReflectDestroyShaderModule(&module);
        }
    }

    for(auto& set : sets) {
        std::cout << " Printing set : " << set.set << std::endl;

        std::vector<VkDescriptorSetLayoutBinding> layoutBindingInfo;

        for(const auto& binding : set.bindings) {
            std::cout << "      Type : " << binding.type << "    Binding locaton : " << binding.binding << std::endl;

            VkDescriptorSetLayoutBinding bindingInfo{};
            bindingInfo.descriptorType = binding.type;
            bindingInfo.binding = binding.binding;
            bindingInfo.stageFlags = binding.stageFlags;
            bindingInfo.descriptorCount = binding.count;

            //TODO add sampler
            //bindingInfo.pImmutableSamplers = sampler;

            layoutBindingInfo.push_back(bindingInfo);
        }

        VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
        layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutCreateInfo.bindingCount = layoutBindingInfo.size();
        layoutCreateInfo.pBindings = layoutBindingInfo.data();
        layoutCreateInfo.pNext = nullptr;

        VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, &set.layout), "Failed to create descriptor set layout");

        for(const auto& shader : set.shaders) {
            std::cout << "      Used by shader : " << shader << std::endl;
        }
    }

    VkDescriptorPoolCreateInfo poolCreateInfo{};
    poolCreateInfo.maxSets = sets.size();
}


Volly::PipelineManager::PipelineManagerImpl::~PipelineManagerImpl() {
    for(const auto& set : sets) vkDestroyDescriptorSetLayout(device, set.layout, nullptr);

    for(const auto& layout: pipelineLayouts) vkDestroyPipelineLayout(device, layout.layout, nullptr);

    vkDestroyPipelineCache(device, cache, nullptr);
}

std::vector<uint32_t> Volly::PipelineManager::PipelineManagerImpl::readShader(const std::string& path) {
    //Load the spv shader file
    std::ifstream shader(path, std::ios::ate | std::ios::binary);
    if (!shader.is_open()) {
        throw std::runtime_error("Failed to open SPIR-V file!");
    }

    size_t fileSize = static_cast<size_t>(shader.tellg());
    if (fileSize % 4 != 0) {
        throw std::runtime_error("SPIR-V file size is not a multiple of 4");
    }

    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
    shader.seekg(0);
    shader.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    shader.close();

    return buffer;
}

void Volly::PipelineManager::PipelineManagerImpl::getDescriptorSets(SpvReflectShaderModule& module, const std::string& path) {
    uint32_t setCount = 0;
    spvReflectEnumerateDescriptorSets(&module, &setCount, nullptr);
    std::vector<SpvReflectDescriptorSet*> spvSets(setCount);
    spvReflectEnumerateDescriptorSets(&module, &setCount, spvSets.data());

    for (const SpvReflectDescriptorSet* spvSet : spvSets) {

        int setIndex = -1;

        for(size_t i = 0; i < sets.size(); i++) {
            if(sets[i].set == spvSet->set) {
                setIndex = i;
                break;
            }
        }
        if(setIndex == -1) {
            sets.push_back({ .set = spvSet->set });
            setIndex = sets.size() - 1;
        }

        sets[setIndex].shaders.push_back(path);

        for (uint32_t i = 0; i < spvSet->binding_count; ++i) {
            const SpvReflectDescriptorBinding* binding = spvSet->bindings[i];

            DescriptorBindingInfo bindingInfo = {
                .binding = binding->binding,
                .type = static_cast<VkDescriptorType>(binding->descriptor_type),
                .count = binding->count,
                .stageFlags = static_cast<VkShaderStageFlags>(module.shader_stage),
            };

            sets[setIndex].pushBack(bindingInfo);
        }
    }
}

void Volly::PipelineManager::PipelineManagerImpl::getVertexInput(SpvReflectShaderModule& module, const std::string& path) {
    uint32_t inputVarCount = 0;
    std::vector<SpvReflectInterfaceVariable*> inputVars;
    spvReflectEnumerateInputVariables(&module, &inputVarCount, nullptr);
    inputVars.resize(inputVarCount);
    spvReflectEnumerateInputVariables(&module, &inputVarCount, inputVars.data());

    for(const auto& inputVar : inputVars) {
        VertexAttributeInfo attributeInfo{};
        attributeInfo.inputAttributeDescription = VkVertexInputAttributeDescription{
            .location = inputVar->location,
            .format = static_cast<VkFormat>(inputVar->format),
        };
        attributeInfo.bindingDescription = {
        };
    }
}

VkShaderModule Volly::PipelineManager::PipelineManagerImpl::createShaderModue(const std::string& path) {
    std::vector<uint32_t> code = readShader(path);

    VkShaderModuleCreateInfo shaderModuleCreateInfo{};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size() * sizeof(uint32_t);
    shaderModuleCreateInfo.pCode = code.data();

    VkShaderModule module;
    VK_CHECK(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &module), "Failed to create shader module")

    return module;
}

Volly::PipelineManager::PipelineManagerImpl::PipelineLayout Volly::PipelineManager::PipelineManagerImpl::createPipelineLayout(std::unordered_set<std::string>&& shaders) {
    for(const auto pipelineLayout : pipelineLayouts) {
        if(std::unordered_set<std::string>(pipelineLayout.shaders.begin(), pipelineLayout.shaders.end()) == shaders) {
            return pipelineLayout;
        }
    }

    std::set<VkDescriptorSetLayout> descriptorLayout;

    for(const auto& set: sets) {
        for(const auto& shader: set.shaders) {
            for(const auto& providedShader: shaders) {
                if(shader == providedShader) descriptorLayout.insert(set.layout);
            }
        }
    }

    VkPipelineLayoutCreateInfo layoutCreateInfo{};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    std::vector<VkDescriptorSetLayout> idkwtftonamethis(descriptorLayout.begin(), descriptorLayout.end());
    layoutCreateInfo.setLayoutCount = idkwtftonamethis.size();
    layoutCreateInfo.pSetLayouts = idkwtftonamethis.data();

    VkPipelineLayout pipelineLayout;
    VK_CHECK(vkCreatePipelineLayout(device, &layoutCreateInfo, nullptr, &pipelineLayout), "Failed to create pipeline layout")

    PipelineLayout newLayout = {
        .layout = pipelineLayout,
        .shaders = std::vector(shaders.begin(), shaders.end()),
    };

    pipelineLayouts.push_back(newLayout);

    return newLayout;
}

Volly::GraphicsPipeline Volly::PipelineManager::PipelineManagerImpl::createGraphicsPipeline(const GraphicsPipelineCreateInfo& createInfo) {
    VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.layout = createPipelineLayout({createInfo.vertexShader, createInfo.fragmentShader}).layout;
    pipelineCreateInfo.pVertexInputState = 0;
}

Volly::ComputePipeline Volly::PipelineManager::PipelineManagerImpl::createComputePipeline(const ComputePipelineCreateInfo& createInfo) {
    VkShaderModule module = createShaderModue(createInfo.computeShader);

    VkPipelineShaderStageCreateInfo shaderStageCreateInfo {};
    shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStageCreateInfo.pName = "main";
    shaderStageCreateInfo.module = module;

    VkComputePipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stage = shaderStageCreateInfo;
    pipelineCreateInfo.layout = createPipelineLayout({createInfo.computeShader}).layout;

    VkPipeline pipeline;
    VK_CHECK(vkCreateComputePipelines(device, cache, 1, &pipelineCreateInfo, nullptr, &pipeline), "Failed to create compute pipeline")

    vkDestroyShaderModule(device, module, nullptr);

    std::unique_ptr<ComputePipeline::ComputePipelineImpl> computePipelineImpl = std::make_unique<ComputePipeline::ComputePipelineImpl>(device, pipeline);

    return ComputePipeline(std::move(computePipelineImpl));
}
