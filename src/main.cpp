#include <volly.hpp>

int main() {
    Volly::Window window("Oinke Poinke");

    Volly::Instance instance({
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

    Volly::PipelineManager pipelineManager = device.createPipelineManager({
        .shaderDirectory = "compiledShaders/",
    });

    Volly::ComputePipeline computePipeline = pipelineManager.createComputePipeline({
        .computeShader = "compiledShaders/test3.spv"
    });
}
