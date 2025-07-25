#pragma once

#include <spvReflect/spirvReflect.h>
#include <pipeline/pipelineManager.hpp>

#include <volk/volk.h>
#include <string>
#include <set>

namespace Volly {
    class PipelineManager::PipelineManagerImpl {
        public:
        PipelineManagerImpl(VkPipelineCache cache, VkDevice device, std::string shaderDirectory);
        ~PipelineManagerImpl();

        struct DescriptorBindingInfo {
            uint32_t set;
            uint32_t binding;
            VkDescriptorType type;

            bool operator<(const DescriptorBindingInfo& other) const noexcept;
            bool operator==(const DescriptorBindingInfo& other) const noexcept;
        };

        private:

        VkPipelineCache cache = VK_NULL_HANDLE;
        VkDevice device;

        std::string shaderDirectory;
        std::set<DescriptorBindingInfo> descriptorBindingInfos;

        void getDescriptorSets(const std::string& path);
    };
}
