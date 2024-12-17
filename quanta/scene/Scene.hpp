#pragma once;

#include "Shapes.hpp"
#include <vector>

namespace Scene
{
    struct SceneGraph
    {
        std::vector<std::shared_ptr<Shapes::RayTarget>> ray_targets;
    };
}