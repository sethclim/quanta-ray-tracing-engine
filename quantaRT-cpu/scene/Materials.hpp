#pragma once
#include "vector3.hpp"
#include "quanta_types.hpp"
#include "utilities/Random.hpp"

namespace Materials
{
    class Material
    {
    public:
        Math::Vector3<float> Albedo = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
        // float EmissionStrength = 0.0f;
        // Math::Vector3<float> EmissionColor = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
        bool GetIsLight() { return isLight; }


        Material(const std::string &name, bool light = false) : _name(name), isLight(light){}
        Material(const std::string& name, const Math::Vector3<float>& albedo, bool light = false) : _name(name), Albedo(albedo), isLight(light) {}

        const std::string &GetName() const { return _name; }
        void SetName(const std::string &name) { _name = name; }

        virtual ~Material() = default;

        virtual bool scatter(const Ray &r_in, const HitInfo &rec, Math::Vector3<float> &attenuation, Ray &scattered) const
        {
            return false;
        }

    private:
        std::string _name;
    protected:
        bool isLight;
    };

    class Emissive : public Material
    {
    public:
        Math::Vector3<float> Albedo = Math::Vector3<float>(1.0f, 1.0f, 1.0f);
        float EmissionStrength = 1.0f;
        Math::Vector3<float> EmissionColor = Math::Vector3<float>(1.0f, 1.0f, 1.0f);

        Emissive(const std::string &name) : Material(name, true) {}
        virtual ~Emissive() = default;

        virtual bool scatter(const Ray &r_in, const HitInfo &rec, Math::Vector3<float> &attenuation, Ray &scattered) const
        {
            return false;
        }
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(const std::string &name) : Material(name) {}

        bool scatter(const Ray &r_in, const HitInfo &hit_info, Math::Vector3<float> &attenuation, Ray &scattered) const override
        {
            auto scatter_direction = Utilities::Random::Random_Unit_Vector() + hit_info.Normal;

            /*auto scatter_direction = hit_info.Normal;*/

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = hit_info.Normal;

            Ray ray;
            ray.Origin = hit_info.HitPoint;
            ray.Direction = scatter_direction;

            scattered = ray;
            attenuation = Albedo;
            return true;
        }
    };

    class Metal : public Material
    {
    public:
        Metal(const std::string &name, const Math::Vector3<float> &albedo, float fuzz) : Material(name, albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const Ray &r_in, const HitInfo &hit_info, Math::Vector3<float> &attenuation, Ray &scattered)
            const override
        {
            auto r_dir = r_in.Direction;
            auto reflected = RayHelpers::Reflect(r_dir, hit_info.Normal);
            reflected = reflected.Normalize() + (Utilities::Random::Random_Unit_Vector() * fuzz);

            Ray ray_scattered;
            ray_scattered.Origin = hit_info.HitPoint;
            ray_scattered.Direction = reflected;

            scattered = ray_scattered;
            attenuation = Albedo;
            return (scattered.Direction.Dot(hit_info.Normal) > 0);
        }


        float fuzz;
    };
}
