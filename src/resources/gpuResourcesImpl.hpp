#pragma once

#include <array>
#include <memory>
#include <stack>

#include <resources/gpuResources.hpp>

namespace Volly {

    template<typename ResourceT> class ResourcePool {
        public:

        static inline constexpr uint32_t MAX_BLOCKS = 100;
        static inline constexpr uint32_t BLOCK_SIZE = 69;

        using block = std::array<std::pair<GpuResourceID, ResourceT>, BLOCK_SIZE>;

        ResourcePool() = default;
        ~ResourcePool() = default;

        GpuResourceID createSlot(ResourceT resource);
        void destroySlot(GpuResourceID id);

        ResourceT retrive(GpuResourceID id);

        void tryToKill(GpuResourceID id);

        private:

        uint32_t bufferID = 0;
        uint32_t blockID = 0;
        std::stack<GpuResourceID> freeIndices = {};
        std::array<std::unique_ptr<block>, MAX_BLOCKS> resData;
    };

    struct Buffer {
    };

    struct Image {

    };

    struct Blas {

    };

    struct Tlas {

    };
}
