#include "core/device.hpp"
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
}
