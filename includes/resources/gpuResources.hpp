#pragma once

#include <atomic>
namespace Volly {

    //Inside index:
    //  1. we allocate the first 16 bits from the right as to the index inside the block
    //  2. the left most bit is a validity bit. 1 means the resource is valid and 0 means the resource is invalid
    //  3. the remianing 15 bits give the index for the block itself
    //  3. reuse count maintins a version number, so onlt the id with the current version is used

    struct GpuResourceID{
        std::atomic<uint32_t> index;
        std::atomic<uint32_t> reuseCount;

        GpuResourceID() = default;
        GpuResourceID(const GpuResourceID& other) noexcept;
        GpuResourceID& operator=(const GpuResourceID& other) noexcept;
    };

    struct BufferID : public GpuResourceID {

    };

    struct ImangeID : public GpuResourceID{

    };

    struct BlasID : public GpuResourceID{

    };

    struct TlasID : public GpuResourceID {

    };

}
