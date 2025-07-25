#version 460

layout(set = 0, binding = 0) buffer Ogga {
    uint i;
} ogga;

layout(set = 0, binding = 2) buffer Pogga {
    uint j;
} pogga;

layout(set = 1, binding = 0, rgba16f) uniform image2D imageFromSet1;

void main() {}
