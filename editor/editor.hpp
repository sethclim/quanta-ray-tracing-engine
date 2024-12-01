#pragma once

#include <vector>
//#include "MapleUI/maple.hpp"


class Maple;


class Editor
{
public:
    Editor();

    std::vector<int> CalculateLayout(int width, int height);
    std::vector<int> RenderEditor();

    std::vector<int> dimensions;

    Maple* maple;
};
