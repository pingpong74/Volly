#include "core/device.hpp"
#include "core/swapchain.hpp"
#include "core/window.hpp"
#include <core/instance.hpp>

int main() {
    Volly::Window window("Oinke Poinke");

    Volly::Instance instance = Volly::Instance::create({
        .name = "Oinke Instance",
        .window = window,
    });

    Volly::Device device = instance.createDevice({
        .name = "Main Device",
    });

    Volly::Swapchain swapchain = device.createSwapchain({
        .name = "Main swapchian",
        .surface = instance.getSurface(),
        .width = 800,
        .height = 600,
    });
}
