#pragma once

#include <volk/volk.h>
#include <stdexcept>
#include <print>


#ifdef DEBUG
    #define VK_CHECK(name, ...) do { \
        VkResult result = name; \
        if(result != VK_SUCCESS) {\
            std::println(__VA_ARGS__); \
            throw std::runtime_error("Fatal error reported");\
        } \
    } while(0);

#else
    #define VK_CHECK(name, err) name;
#endif

constexpr uint32_t validationLayersCount = 1;
constexpr const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

constexpr uint32_t initialWidth = 800;
constexpr uint32_t initialHeight = 600;
