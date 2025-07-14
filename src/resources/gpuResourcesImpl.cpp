#include "gpuResourcesImpl.hpp"
#include "resources/gpuResources.hpp"

template<typename ResourceT>
Volly::GpuResourceID Volly::ResourcePool<ResourceT>::createSlot(ResourceT res) {
    if(freeIndices.empty()) {
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
        }

        uint32_t index = ((blockID) << 16) & (bufferID) & (1 << 31);
        GpuResourceID id{};
        id.index.store(index);
        id.index.store(0);

        return id;
    }
}

template<typename ResourceT>
void Volly::ResourcePool<ResourceT>::destroySlot(GpuResourceID id) {

}
