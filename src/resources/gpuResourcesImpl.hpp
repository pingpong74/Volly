#pragma once

#include "core/device.hpp"
#include <array>
#include <memory>
#include <stack>

#include <resources/gpuResources.hpp>
#include <volk/volk.h>
#include <vulkan/vulkan_core.h>

namespace Volly {

    template<typename ResourceT> class ResourcePool {
        public:

        static inline constexpr uint32_t MAX_BLOCKS = 100;
        static inline constexpr uint32_t BLOCK_SIZE = 69;

        using block = std::array<std::pair<GpuResourceID, ResourceT>, BLOCK_SIZE>;

        ResourcePool() {
            resData[0] = std::make_unique<block>();
        }

        ~ResourcePool() = default;

        GpuResourceID createSlot(ResourceT resource) {
            if(!freeIndices.empty()) {
                GpuResourceID id = freeIndices.top();
                freeIndices.pop();

                id.reuseCount += 1;
                return id;
            }
            else {
                //Ofc this is wrong, but it works for now ig
                if(bufferID > BLOCK_SIZE) {
                    bufferID = 0;
                    blockID += 1;
                    resData[blockID] = std::make_unique<block>();
                }

                uint32_t index = ((blockID) << 16) & (bufferID) & (1 << 31);
                GpuResourceID id{};
                id.index.store(index);
                id.index.store(0);

                return id;
            }
        }

        void zombiefiy(GpuResourceID& id) {
            id.index ^= (1 << 31);
            freeIndices.push(id);
        }

        void destroySlot(GpuResourceID id);

        inline ResourceT retrive(GpuResourceID& id) {return resData[ (id.index >> 16) & 0b1111111111111111][(0b111111111111111 & id.index)]; }

        private:

        uint32_t bufferID = 0;
        uint32_t blockID = 0;
        std::stack<GpuResourceID> freeIndices = {};
        std::array<std::unique_ptr<block>, MAX_BLOCKS> resData;
    };

    struct Buffer {
        VkBuffer handle = VK_NULL_HANDLE;
        VmaAllocation bufferAllocation = VK_NULL_HANDLE;
        VmaAllocationInfo bufferAllocationInfo = {};
    };

    struct Image {

    };

    struct Blas {

    };

    struct Tlas {

    };
}
