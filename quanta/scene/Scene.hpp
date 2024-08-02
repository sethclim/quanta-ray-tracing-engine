#pragma once;

#include "Shapes.hpp"
#include <vector>

namespace Scene
{
    struct SceneGraph
    {
        std::vector<Shapes::Sphere> Spheres;
    };
}