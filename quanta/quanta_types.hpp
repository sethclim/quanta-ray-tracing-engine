#pragma once

#include "vector3.hpp"
// #include "scene/Shapes.hpp"

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

inline HitInfo Miss()
{
    HitInfo info{};
    info.HitPoint = Math::Vector3<float>(-1.0f, -1.0f, -1.0f);
    return info;
}

namespace RayHelpers
{
    inline Math::Vector3<float> Reflect(Math::Vector3<float> &v, const Math::Vector3<float> &n)
    {
        // TODO make subtract work on const
        return v - (n * 2 * v.Dot(n));
    }
}