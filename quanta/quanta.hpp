#pragma once;

#include <cstdint>
#include <tgmath.h>
#include "vector3.hpp"
#include "scene/Scene.hpp"
#include <scene/Shapes.hpp>

#include <iostream>

const uint32_t RED = 0x000000ff;
const uint32_t BLUE = 0x00ff0000;
const uint32_t BLACK = 0x00000000;

struct Ray
{
    Math::Vector3<float> Origin;
    Math::Vector3<float> Direction;
};

struct HitInfo
{
    Math::Vector3<float> HitPoint;
    Math::Vector3<float> Normal;
    Scene::Material      Material;
    int ObjectID;
};

class Renderer
{
public:
    Renderer(const Scene::SceneGraph& scene)
    {
        m_Scene = scene;
    }

    uint32_t PerPixel(float x, float y);
    HitInfo TraceRay(const Ray &ray);

private:
    Scene::SceneGraph m_Scene;
};