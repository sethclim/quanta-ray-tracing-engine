#include "quanta.hpp"

static uint32_t ConvertToRGBA(const Math::Vector3<float> &color)
{
    uint8_t r = (color.x * 255.0f);
    uint8_t g = (color.y * 255.0f);
    uint8_t b = (color.z * 255.0f);
    uint8_t a = (255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}

/// <summary>
/// Finds the reflected ray
/// </summary>
/// <param name="N">Normal</param>
/// <param name="Ri">Incident Ray</param>
/// <returns>reflected ray</returns>
Math::Vector3<float> Reflect(Math::Vector3<float> &N, Math::Vector3<float> &Ri)
{
    // Rr = Ri - 2 N (Ri . N)
    return Ri - (N * 2.0f * Math::Dot(Ri, N));
}

const int MAX_BOUNCES = 3;
const int SAMPLES_PER_PIXEL = 10;

Math::Vector3<float> Renderer::PerPixel(float image_x, float image_y)
{
    // convert to 0,0 in the centre
    float coordX = image_x * 2.0f - 1.0f;
    float coordY = image_y * 2.0f - 1.0f;

    Ray ray;
    ray.Origin = Math::Vector3<float>(0.0f, 0.0f, 2.0f);
    ray.Direction = Math::Vector3<float>(coordX, coordY, -1.0f);

    Math::Vector3<float> pixel_color(0, 0, 0);
    for (int sample = 0; sample < SAMPLES_PER_PIXEL; sample++)
    {

        Math::Vector3<float> incomingLight = Math::Vector3<float>(0, 0, 0);
        Math::Vector3<float> rayColor = Math::Vector3<float>(1, 1, 1);

        for (int i = 0; i < MAX_BOUNCES; i++)
        {
            HitInfo info = TraceRay(ray);

            if (info.HitPoint == Math::Vector3<float>(-1, -1, -1))
                break;

            if (i == 0 && info.ObjectID == 2222)
                break;

            ray.Origin = (info.HitPoint + 0.001f);
            // ray.Direction = Reflect(info.Normal, ray.Direction);
            ray.Direction = Utilities::Random::Random_On_Hemisphere(info.Normal);

            Math::Vector3<float> emittedLight = info.Material.EmissionColor * info.Material.EmissionStrength;

            rayColor *= info.Material.Color;

            incomingLight += rayColor * emittedLight;
        }

        pixel_color += incomingLight;
    }

    return pixel_color;
}

HitInfo Renderer::TraceRay(const Ray &ray)
{
    for (const auto& object : m_Scene.ray_targets)
    {
        //Scene::Shapes::RayTarget sphere = m_Scene.ray_targets[i];

        HitInfo info = object->hit(ray);

        if (info.HitPoint == Math::Vector3<float>(-1, -1, -1))
            continue;

        return info;
    }

    HitInfo hitInfo;
    hitInfo.HitPoint = Math::Vector3<float>(-1, -1, -1);
    // All misses
    return hitInfo;
}
