#pragma once

#include "vector3.hpp"
#include "scene/Shapes.hpp"

struct Ray
{
    Math::Vector3<float> Origin;
    Math::Vector3<float> Direction;
};

namespace Scene
{
    struct Material
    {
        Math::Vector3<float> Color = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
        float EmissionStrength = 0.0f;
        Math::Vector3<float> EmissionColor = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
    };
}

struct HitInfo
{
    Math::Vector3<float> HitPoint;
    Math::Vector3<float> Normal;
    Scene::Material Material;
    int ObjectID;
};