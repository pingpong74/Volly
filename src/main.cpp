#include "core/device.hpp"
#include <volly.hpp>

int main() {
    Volly::Window window("Oinke Poinke");

    Volly::Instance instance = Volly::Instance::create({
        .name = "Oinke Instance",
        .window = window,
    });

    Volly::Device device = instance.createDevice({
        .name = "Poinke Device",
    });

    Volly::Swapchain swapchain = device.createSwapchain({
        .name = "Dooinke swapchian",
        .surface = instance.getSurface(),
    });

    Volly::BufferID id = device.createBuffer({
        .name = "Test Buffer",
        .size = 100,
        .usageFlags = Volly::BufferUsageFlags::transferDst ,
        .memoryUsage = Volly::MemoryUsage::preferAuto,
        .allocationFlags = Volly::AllocationCreateFlags::createMapped | Volly::AllocationCreateFlags::sequenctialWriteBit,
    });

    Volly::PipelineManager pipelineManager = device.createPipelineManager({
        .shaderDirectory = "compiledShaders/",
    });

    Volly::ComputePipeline computePipeline = pipelineManager.createComputePipeline({
        .computeShader = "compiledShaders/test3.spv"
    });
}
