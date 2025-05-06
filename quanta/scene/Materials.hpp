#pragma once
#include "vector3.hpp"
#include "quanta_types.hpp"
#include "utilities/Random.hpp"

namespace Materials
{
    class Material
    {
    public:
        Math::Vector3<float> Color = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
        float EmissionStrength = 0.0f;
        Math::Vector3<float> EmissionColor = Math::Vector3<float>(1.0f, 1.0f, 1.0f);

        virtual ~Material() = default;

        virtual bool scatter(const Ray &r_in, const HitInfo &rec, Math::Vector3<float> &attenuation, Ray &scattered) const
        {
            return false;
        }
    };

    class Lambertian : public Material
    {
     public:
         bool scatter(const Ray &r_in, const HitInfo &hit_info, Math::Vector3<float> &attenuation, Ray &scattered) const override
         {
             auto scatter_direction = Utilities::Random::Random_Unit_Vector() + hit_info.Normal;


            /*auto scatter_direction = hit_info.Normal;*/

            // Catch degenerate scatter direction
            if (Math::near_zero(scatter_direction))
                scatter_direction = hit_info.Normal;

            Ray ray;
            ray.Origin = hit_info.HitPoint;
            ray.Direction = scatter_direction;

            scattered = ray;
            attenuation = Color;
            return true;
        }
    };

    class Metal : public Material
    {
     public:
         Metal(const Math::Vector3<float> &albedo) : albedo(albedo) {}

        bool scatter(const Ray &r_in, const HitInfo &hit_info, Math::Vector3<float> &attenuation, Ray &scattered)
            const override
        {
            auto r_dir = r_in.Direction;
            auto reflected = RayHelpers::Reflect(r_dir, hit_info.Normal);

            Ray ray_scattered;
            ray_scattered.Origin = hit_info.HitPoint;
            ray_scattered.Direction = reflected;

            scattered = ray_scattered;
            attenuation = albedo;
            return true;
        }

     private:
         Math::Vector3<float> albedo;
     };
}
