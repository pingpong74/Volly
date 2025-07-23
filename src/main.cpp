#include "core/device.hpp"
#include <volly.hpp>

int main() {
    Volly::Window window("Oinke Poinke");

    Volly::Instance instance = Volly::Instance::create({
        .name = "Oinke Instance",
        .window = window,
        .enableValidationLayers = false
    });

    Volly::Device device = instance.createDevice({
        .name = "Main Device",
    });

    Volly::Swapchain swapchain = device.createSwapchain({
        .name = "Main swapchian",
        .surface = instance.getSurface(),
    });

    Volly::BufferID id = device.createBuffer({
        .name = "Test Buffer",
        .size = 100,
        .usageFlags = Volly::transferSrc ,
        .memoryUsage = Volly::preferAuto,
        .allocationFlags = Volly::createMapped | Volly::sequenctialWriteBit,
    });
}
