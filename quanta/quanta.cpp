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

const int MAX_BOUNCES = 2;
const int SAMPLES_PER_PIXEL = 100;

uint32_t Renderer::PerPixel(float image_x, float image_y)
{
    // convert to 0,0 in the centre
    float coordX = image_x * 2.0f - 1.0f;
    float coordY = image_y * 2.0f - 1.0f;

    Ray ray;
    ray.Origin = Math::Vector3<float>(0.0f, 0.0f, 2.0f);
    ray.Direction = Math::Vector3<float>(coordX, coordY, -1.0f);

    Math::Vector3<float> pixel_color(0, 0, 0);
    for (int sample = 0; sample < SAMPLES_PER_PIXEL; sample++) {

        Math::Vector3<float> incomingLight = Math::Vector3<float>(0, 0, 0);
        Math::Vector3<float> rayColor = Math::Vector3<float>(1, 1, 1);

        for (int i = 0; i < MAX_BOUNCES; i++)
        {
            HitInfo info = TraceRay(ray);

            if (info.HitPoint == Math::Vector3<float>(-1, -1, -1))
            {
                break;
            }

            if (i == 0 && info.ObjectID == 2222)
            {
                break;
            }

       /*     if (i > 0 && info.ObjectID == 2222)
            {
                std::cout << "HIT THE LIGHT" << std::endl;
            }*/

            /* if(i > 0)
                 std::cout << "[Hit] index: " << i << " ID: " << info.ObjectID << " x " << image_x << " y " << image_y << std::endl;*/

            ray.Origin = (info.HitPoint + 0.001f);
            //ray.Direction = Reflect(info.Normal, ray.Direction);
            ray.Direction = Utilities::Random::random_on_hemisphere(info.Normal);

            //ray.Direction = info.Normal;

            //std::cout << "ray.Direction: " << ray.Direction.ToString() << std::endl;

            Math::Vector3<float> emittedLight = info.Material.EmissionColor * info.Material.EmissionStrength;

            //incomingLight += emittedLight * rayColor;
            //incomingLight = emittedLight;

            rayColor *= info.Material.Color;

            /*if (info.ObjectID == 0)
                std::cout << "Hit: id " << i << " " << info.ObjectID << incomingLight.ToString() << std::endl;*/

                //Math::Vector3<float> res = Math::Clamp(info.Normal, 0.0f, 1.0f);
            if (i == 0)
            {
                //incomingLight += (rayColor * emittedLight);
                incomingLight += rayColor * emittedLight;
                //incomingLight += Math::Clamp(info.Normal, 0.0f, 1.0f);
            }
            else
            {
                //incomingLight += emittedLight;
                incomingLight += (rayColor * emittedLight);
                //incomingLight += rayColor;
            }
        }
        pixel_color += incomingLight;
    }

    return ConvertToRGBA(pixel_color);
}

HitInfo Renderer::TraceRay(const Ray &ray)
{
    for (int i = 0; i < m_Scene.Spheres.size(); i++)
    {
        Scene::Shapes::Sphere sphere = m_Scene.Spheres[i];

        Math::Vector3<float> offsetRayOrigin = (Math::Vector3<float>)ray.Origin - sphere.Origin;

        float a = Math::LengthSquared(ray.Direction);
        float b = 2.0f * Math::Dot(offsetRayOrigin, ray.Direction);
        float c = Math::LengthSquared(offsetRayOrigin) - Math::Sqr(sphere.Radius);

        float discriminant = Math::Sqr(b) - 4.0f * a * c;

        if (discriminant < 0.0f)
            continue;

        // int ans = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
        float t0 = (-b + std::sqrt(discriminant)) / (2.0f * a);
        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);

        HitInfo hitInfo;

        hitInfo.HitPoint = (Math::Vector3<float>)ray.Origin + (Math::Vector3<float>)ray.Direction * t1;

        hitInfo.Normal = Math::Normalize(hitInfo.HitPoint - sphere.Origin);

        hitInfo.Material = sphere.Material;

        hitInfo.ObjectID = sphere.id;

        return hitInfo;
    }

    HitInfo hitInfo;
    hitInfo.HitPoint = Math::Vector3<float>(-1, -1, -1);
    // All misses
    return hitInfo;
}
