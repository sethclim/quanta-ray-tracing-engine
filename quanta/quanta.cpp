#include "quanta.hpp"

/// <summary>
/// Finds the reflected ray
/// </summary>
/// <param name="N">Normal</param>
/// <param name="Ri">Incident Ray</param>
/// <returns>reflected ray</returns>
Math::Vector3<float> Reflect(Math::Vector3<float> &N, Math::Vector3<float> &Ri)
{
    // Rr = Ri - 2 N (Ri . N)
    return Ri - (N * 2.0f * Ri.Dot(N));
}

void Renderer::SetRenderSettings(uint32_t samplesPerPixel, uint32_t maxBounces)
{
    _samplesPerPixel = samplesPerPixel;
    _maxBounces = maxBounces;
}

Math::Vector3<float> Renderer::PerPixel(float image_x, float image_y, bool debug)
{

    // convert to 0,0 in the centre
    float coordX = image_x * 2.0f - 1.0f;
    float coordY = image_y * 2.0f - 1.0f;

    // if (debug)
    //     std::cout << "coordX " << coordX << "coordY " << coordY << std::endl;

    Math::Vector3<float> camera = Math::Vector3<float>(0.0f, 0.0f, 2.0f);

    Ray ray;
    ray.Origin = camera;
    ray.Direction = Math::Vector3<float>(coordX, coordY, -1.0f).Normalize();

    // if(debug)
    //     addDebugLine(camera, ray.Direction * 2);

    Math::Vector3<float> pixel_color(0, 0, 0);

    for (int sample = 0; sample < _samplesPerPixel; sample++)
    {
        Math::Vector3<float> incomingLight = Math::Vector3<float>(0, 0, 0);
        Math::Vector3<float> rayColor = Math::Vector3<float>(1, 1, 1);

        for (int i = 0; i < _maxBounces; i++)
        {
            HitInfo info{};
            bool res = TraceRay(ray, info, i == 0);

            if (!res)
            {
                if (i == 0)
                    return Math::Vector3<float>(0.337, 0.576, 0.961);

                break;
            }


            if (debug && i > 0 && sample == 0)
                addDebugLine(ray.Origin, info.HitPoint);

            Math::Vector3<float> attenuation;
            Ray ray2;
            if (info.Material->scatter(ray, info, attenuation, ray2))
            {
                // Math::Vector3<float>
                //     emittedLight = info.Material->EmissionColor * info.Material->EmissionStrength;

                rayColor *= attenuation;
                // incomingLight += rayColor * emittedLight;
                incomingLight += rayColor;

                ray.Origin = ray2.Origin;
                ray.Direction = ray2.Direction;
            }
            else
            {
                std::shared_ptr<Materials::Material> mat = info.Material;
                auto emissive = std::dynamic_pointer_cast<Materials::Emissive>(mat);

                if (emissive)
                {
                    Math::Vector3<float>
                        emittedLight = emissive->EmissionColor * emissive->EmissionStrength;
                    incomingLight += rayColor * emittedLight;
                }
            }
        }

        pixel_color += incomingLight;

        // pixel_color = rayColor;
    }

    return pixel_color;
}

bool Renderer::TraceRay(const Ray &ray, HitInfo &hitinfo, bool isCameraRay)
{
    Utilities::Interval ray_t = Utilities::Interval(0.001, std::numeric_limits<double>::infinity());
    HitInfo closestHit = Miss();

    bool hit = false;

    for (const auto &object : m_Scene.ray_targets)
    {
        HitInfo info{};

        if (!object->hit(ray, ray_t, info))
        {
            continue;
        }
        else
        {
            Materials::Material mat = *info.Material.get();
            if (isCameraRay && mat.GetIsLight())
            {
                continue;
            }
            closestHit = info;
            hit = true;
        }
    }

    hitinfo = closestHit;
    return hit;
}
