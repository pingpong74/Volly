#include "pipelineManagerImpl.hpp"
//#include "voxyConfig.hpp"

#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <iostream>


//If other is bigger return true
bool Volly::PipelineManager::PipelineManagerImpl::DescriptorBindingInfo::operator<(const DescriptorBindingInfo& other) const noexcept {
    if(set < other.set) return true;
    else if(set == other.set) return binding < other.binding;
    else return false;
}

bool Volly::PipelineManager::PipelineManagerImpl::DescriptorBindingInfo::operator==(const DescriptorBindingInfo& other) const noexcept {
    return set == other.set && binding == other.binding && type == other.type;
}

Volly::PipelineManager::PipelineManagerImpl::PipelineManagerImpl(VkPipelineCache cache, VkDevice device, std::string shaderDirectory) : cache(cache), device(device), shaderDirectory(shaderDirectory) {
    for(const auto& file: std::filesystem::directory_iterator(shaderDirectory)) {
        if(file.path().extension() == ".spv") {
            getDescriptorSets(file.path().string());
        }
    }

    for(const auto& info: descriptorBindingInfos) {
        std::cout << "Set : " << info.set << "   Binding :" << info.binding << "   Type :" << info.type << std::endl;
    }
}

void Volly::PipelineManager::PipelineManagerImpl::getDescriptorSets(const std::string& path) {
    //Load the spv shader file
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open SPIR-V file!");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    if (fileSize % 4 != 0) {
        throw std::runtime_error("SPIR-V file size is not a multiple of 4");
    }

    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();

    //create a spv shader module
    SpvReflectShaderModule module;
    if(spvReflectCreateShaderModule(fileSize, buffer.data(), &module) != SPV_REFLECT_RESULT_SUCCESS) {
        throw std::runtime_error("Failed to generate module from spv data");
    }

    //Get the descriptor sets
    uint32_t setCount = 0;
    spvReflectEnumerateDescriptorSets(&module, &setCount, nullptr);
    std::vector<SpvReflectDescriptorSet*> sets(setCount);
    spvReflectEnumerateDescriptorSets(&module, &setCount, sets.data());

    for (const SpvReflectDescriptorSet* set : sets) {
        for (uint32_t i = 0; i < set->binding_count; ++i) {
            const SpvReflectDescriptorBinding* binding = set->bindings[i];

            DescriptorBindingInfo bindingInfo = {
                .set = set->set,
                .binding = binding->binding,
                .type = (VkDescriptorType)binding->descriptor_type,
            };

            descriptorBindingInfos.insert(bindingInfo);
        }
    }
}

Volly::PipelineManager::PipelineManagerImpl::~PipelineManagerImpl() {
    vkDestroyPipelineCache(device, cache, nullptr);
}
