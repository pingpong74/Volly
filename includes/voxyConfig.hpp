#pragma once

#include <volk/volk.h>
#include <stdexcept>

#ifdef DEBUG
    #define VK_CHECK(name, err) if(name != VK_SUCCESS) throw std::runtime_error(err);
#else
    #define VK_CHECK(name, err) name;
#endif

constexpr uint32_t validationLayersCount = 1;
constexpr const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

constexpr uint32_t initialWidth = 800;
constexpr uint32_t initialHeight = 600;
