#pragma once;

#include "vector3.hpp"

namespace Scene
{
    namespace Shapes
    {
        struct Sphere
        {
            float Radius = 0.5f;
            Math::Vector3<float> Origin;
        };

    }
}