#pragma once

#include "vector3.hpp"
#include "quanta_types.hpp"
#include "Utilities/Interval.hpp"
#include "scene/Materials.hpp"

namespace Scene
{
    namespace Shapes
    {
        class RayTarget
        {
        public:
            virtual ~RayTarget() = default;
            virtual HitInfo hit(Ray ray, Utilities::Interval ray_t) = 0;
        };

        class Sphere : public RayTarget
        {
        public:
            float Radius = 0.5f;
            Math::Vector3<float> Origin;
            //Materials::Material Material;
            int id;

            ~Sphere() {}

            HitInfo hit(Ray ray, Utilities::Interval ray_t)
            {
                Math::Vector3<float> offsetRayOrigin = (Math::Vector3<float>)ray.Origin - Origin;

                float a = Math::LengthSquared(ray.Direction);
                float b = 2.0f * Math::Dot(offsetRayOrigin, ray.Direction);
                float c = Math::LengthSquared(offsetRayOrigin) - Math::Sqr(Radius);

                float discriminant = Math::Sqr(b) - 4.0f * a * c;
                if (discriminant < 0.0f)
                {
                    HitInfo info{};
                    info.HitPoint = Math::Vector3(-1.0f, -1.0f, -1.0f);
                    return info;
                }

                double t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);

                if (!ray_t.surrounds(t1))
                {
                    // int ans = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
                    double t0 = (-b + std::sqrt(discriminant)) / (2.0f * a);
                    if (!ray_t.surrounds(t0))
                    {
                        HitInfo info{};
                        info.HitPoint = Math::Vector3(-1.0f, -1.0f, -1.0f);
                        return info;
                    }
                }

                HitInfo hitInfo{};

                hitInfo.HitPoint = (Math::Vector3<float>)ray.Origin + (Math::Vector3<float>)ray.Direction * t1;
                hitInfo.Normal = Math::Normalize(hitInfo.HitPoint - Origin);
                hitInfo.Material = Material;
                hitInfo.ObjectID = id;

                return hitInfo;
            }

            std::shared_ptr<Materials::Material> Material;
        };
    }
}