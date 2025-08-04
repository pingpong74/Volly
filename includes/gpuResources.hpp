#pragma once

#include <cstdint>
#include <array>
#include <memory>
#include <optional>
#include <stack>
#include <stdexcept>
#include <utility>

namespace Volly {
    class Device;

    // Last 16 digits are page id
    // precedding those, 16 digits are id inside the page
    // the next 16 bits are the version
    // Validity bit    Padding bits   Version Bits     BlockID bits    PageID bits
    //      1              15                16              16             16       <- number of bits allocated
    // the very left most bit is validity bit (0 means invalid 1 means valid)
    struct GpuResourceID {
        uint64_t id = 0;

        inline uint16_t getPageID() { return id & 0b1111111111111111; }
        inline uint16_t getBlockID() { return (id >> 16) & 0b1111111111111111; }
        inline uint16_t getVersion() { return (id >> 32) & 0b1111111111111111; }

        inline bool isValid() { return (id >> 63) & 1; }

        inline bool operator==(const GpuResourceID& other) const = default;
    };

    struct BufferID : public GpuResourceID {
        ~BufferID();
    };

    struct ImageID : public GpuResourceID {

    };

    struct SamplerID : public GpuResourceID {

    };

    template <typename Resource, typename  ResourceID>
    class GpuResourcePool {
        public:

        static constexpr uint32_t PAGE_SIZE = 10;
        static constexpr uint32_t MAX_PAGES = 10;

        struct Slot {
            std::optional<Resource> resource;

            uint32_t refCout = 0;
            uint32_t version = 0;
        };

        using ResourceBlock = std::array<std::pair<Slot, ResourceID>, PAGE_SIZE>;

        GpuResourcePool() {
            for(auto& page: data) page = nullptr;
            data[0] = std::make_unique<ResourceBlock>();
        }

        ResourceID create(const Resource& resource) {
            if(freeIDs.empty()) {
                uint16_t pageID = currID.getPageID();
                uint16_t blockID = currID.getBlockID();

                if(pageID == MAX_PAGES - 1) throw std::runtime_error("Resource Pool over flow");

                if(blockID >= PAGE_SIZE) {
                    pageID += 1;
                    data[pageID] == std::make_unique<ResourceBlock>();
                    blockID = 0;
                }

                ResourceID id = { ((uint64_t)1 << 63) | ((uint64_t)blockID << 16) | ((uint64_t)pageID) };
                blockID += 1;

                (*data[pageID])[blockID] = std::make_pair<Slot, ResourceID>({resource, 0}, id);
                return id;
            }
            else {
                ResourceID reuseID = freeIDs.top();
                uint16_t pageID = currID.getPageID();
                uint16_t blockID = currID.getBlockID();
                freeIDs.pop();

                ResourceID id = {((uint64_t)1 << 63) | ((id.getVersion() + 1) << 32) | (blockID << 16) | (pageID)};

                (*data[pageID])[blockID] = std::make_pair<Slot, ResourceID>({resource, 0}, id);

                return id;
            }
        }

        Resource* get(ResourceID id) {
            return (*data[id.getPageID])[id.getBlockID].first;
        }

        void destroy(const ResourceID& id) {

        }

        private:
        ResourceID currID = {(uint64_t)1 << 63};
        std::array<std::unique_ptr<ResourceBlock> , MAX_PAGES> data;
        std::stack<ResourceID> freeIDs;
    };
}
