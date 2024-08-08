#pragma once;

#include "vector3.hpp"

namespace Scene
{
    struct Material
    {
        Math::Vector3<float> Color = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
        float EmissionStrength = 0.0f;
        Math::Vector3<float> EmissionColor = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
    };

    namespace Shapes
    {
        struct Sphere
        {
            float Radius = 0.5f;
            Math::Vector3<float> Origin;
            Material Material;
        };
    }
}