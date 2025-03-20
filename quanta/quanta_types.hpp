#pragma once

#include "vector3.hpp"
//#include "scene/Shapes.hpp"

namespace Materials
{
    class Material;
};

struct Ray
{
    Math::Vector3<float> Origin;
    Math::Vector3<float> Direction;
};

struct HitInfo
{
    Math::Vector3<float> HitPoint;
    Math::Vector3<float> Normal;
    std::shared_ptr<Materials::Material> Material;
    int ObjectID;
};

namespace RayHelpers
{
    inline Math::Vector3<float> Reflect(Math::Vector3<float> &v, const Math::Vector3<float> &n)
    {
        // TODO make subtract work on const
        return (v - 2) * 2 * Dot(v, n);
    }
}