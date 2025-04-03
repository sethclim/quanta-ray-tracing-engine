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
    return Ri - (N * 2.0f * Math::Dot(Ri, N));
}

const int MAX_BOUNCES = 2;
const int SAMPLES_PER_PIXEL = 1;

Math::Vector3<float> Renderer::PerPixel(float image_x, float image_y, bool debug)
{

    // convert to 0,0 in the centre
    float coordX = image_x * 2.0f - 1.0f;
    float coordY = image_y * 2.0f - 1.0f;

    if (debug)
        std::cout << "coordX " << coordX << "coordY " << coordY << std::endl;

    Math::Vector3<float> camera = Math::Vector3<float>(0.0f, 0.0f, 2.0f);

    Ray ray;
    ray.Origin = camera;
    ray.Direction = Math::Normalize(Math::Vector3<float>(coordX, coordY, -1.0f));

    //if(debug)
    //    addDebugLine(camera, ray.Direction * 2);

    Math::Vector3<float> pixel_color(0, 0, 0);

    for (int sample = 0; sample < SAMPLES_PER_PIXEL; sample++)
    {
        Math::Vector3<float> incomingLight = Math::Vector3<float>(0, 0, 0);
        Math::Vector3<float> rayColor = Math::Vector3<float>(1, 1, 1);

        for (int i = 0; i < MAX_BOUNCES; i++)
        {
            HitInfo info = TraceRay(ray);

            if (info.HitPoint == Math::Vector3<float>(-1, -1, -1))
            {
                if (i == 0)
                    rayColor = Math::Vector3<float>(0, 0, 0);
                break; 
            }

           if (i == 0 && info.ObjectID == 2222)
                break;
            /*
            if (i == 1)
            {
                std::cout << "hit: image_x: " << image_x << " image_y " << image_y << std::endl;
            }*/ 

         /*   if (i > 0)
                std::cout << "hit: object: " << info.ObjectID << "x: " << info.HitPoint.x << " y " << info.HitPoint.y <<  " z " << info.HitPoint.z << std::endl;*/

           if (debug && i == 0)
           {
                //std::cout << "hit: object: " << info.ObjectID << "x: " << info.HitPoint.x << " y " << info.HitPoint.y << " z " << info.HitPoint.z << std::endl; 
                addDebugLine(info.HitPoint, info.Normal * 0.02);
           }

           if (info.ObjectID == 2222)
               rayColor = Math::Vector3<float>(0, 0.2, 0.4);
           else
               rayColor = Math::Vector3<float>(0.2, 0.4, 0.2);

            // ray.Origin = (info.HitPoint + 0.001f);
            // // ray.Direction = Reflect(info.Normal, ray.Direction);
            // // ray.Direction = Utilities::Random::Random_On_Hemisphere(info.Normal);
            // ray.Direction = info.Normal + Utilities::Random::Random_Unit_Vector();

            Math::Vector3<float> attenuation;
            Ray ray2;
            if (info.Material->scatter(ray, info, attenuation, ray2))
            {
                //Math::Vector3<float>
                //    emittedLight = info.Material->EmissionColor * info.Material->EmissionStrength;

                //rayColor *= attenuation;

                //incomingLight += rayColor * emittedLight;

                ray.Origin = ray2.Origin;
                ray.Direction = ray2.Direction;
            }
            else
            {
                //Math::Vector3<float>
                //    emittedLight = info.Material->EmissionColor * info.Material->EmissionStrength;
                //incomingLight += rayColor * emittedLight;
            }
        }

  /*      pixel_color += rayColor;*/

        pixel_color = rayColor;
       
    }

    return pixel_color;
}

HitInfo Renderer::TraceRay(const Ray &ray)
{
    Utilities::Interval ray_t = Utilities::Interval(0.001, std::numeric_limits<double>::infinity());
    double closest_so_far = ray_t.max;
    for (const auto &object : m_Scene.ray_targets)
    {

        HitInfo info = object->hit(ray, Utilities::Interval(ray_t.min, closest_so_far));

        if (info.HitPoint == Math::Vector3<float>(-1, -1, -1))
            continue;

        return info;
    }

    HitInfo hitInfo;
    hitInfo.HitPoint = Math::Vector3<float>(-1, -1, -1);
    // All misses
    return hitInfo;
}
