#pragma once;

#include <cstdint>
#include <tgmath.h>
#include <iostream>
#include <limits>

#include "quanta_types.hpp"
#include "vector3.hpp"
#include "scene/Scene.hpp"
#include <scene/Shapes.hpp>
#include "scene/Materials.hpp"
#include <utilities/Random.hpp>
#include "Utilities/Interval.hpp"

const uint32_t RED = 0x000000ff;
const uint32_t BLUE = 0x00ff0000;
const uint32_t BLACK = 0x00000000;

namespace Utilities
{
    struct DebugLine {
        Math::Vector3<float> start;
        Math::Vector3<float> end;
    };
}

class Renderer
{
public:
    Renderer(const Scene::SceneGraph &scene)
    {
        m_Scene = scene;
    }

    Math::Vector3<float> PerPixel(float x, float y, bool debug);
    HitInfo TraceRay(const Ray &ray);

    std::vector<Utilities::DebugLine> GetDebugLines() { return debugLines; }

private:
    void addDebugLine(Math::Vector3<float> start, Math::Vector3<float> end) {
        debugLines.push_back({ start, end });
        //std::cout << "Debug Line Length " << debugLines.size() << std::endl;
    }

    Scene::SceneGraph m_Scene;  
    std::vector<Utilities::DebugLine> debugLines;
};
