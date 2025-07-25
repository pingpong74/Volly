#version 460

layout(set = 0, binding = 0) buffer Ogga {
    uint i;
} ogga;

layout(set = 0, binding = 1, rgba16f) uniform image2D imageFromSet0;

void main() {}
