#pragma once

#include <vector>
#include "../app/src/VulkanBackend.hpp"
// #include "../app/src/VulkanBackend.hpp"
// #include "MapleUI/maple.hpp"

class Maple;

struct DrawData
{
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
};

class Editor
{
    //const std::vector<Vertex> vertices = {
    //    {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    //    {{0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    //    {{0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    //    {{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };

public:
    Editor();

    std::vector<int> CalculateLayout(int width, int height);
    DrawData RenderEditor();

    std::vector<int> dimensions;

    Maple *maple;
};
