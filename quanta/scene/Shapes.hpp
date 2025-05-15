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
            virtual bool hit(Ray ray, Utilities::Interval &ray_t, HitInfo& hitInfo) = 0;
        };

        class Sphere : public RayTarget
        {
        public:
            float Radius = 0.5f;
            Math::Vector3<float> Origin;
            int id;

            ~Sphere() {}

            bool hit(Ray ray, Utilities::Interval &ray_t, HitInfo& hitInfo)
            {
                Math::Vector3<float> offsetRayOrigin = (Math::Vector3<float>)ray.Origin - Origin;

                // std::cout << "offsetRayOrigin [x:" << offsetRayOrigin.x << " y: " << offsetRayOrigin.y << " z: " << offsetRayOrigin.z << "]" << std::endl;

                float a = ray.Direction.LengthSquared();
                float b = 2.0f * offsetRayOrigin.Dot(ray.Direction);
                float c = offsetRayOrigin.LengthSquared() - Math::Sqr(Radius);

                float discriminant = Math::Sqr(b) - 4.0f * a * c;
                if (discriminant < 0.0f)
                {
                    hitInfo = Miss();
                    return false;
                }

                double sqrt_disc = std::sqrt(discriminant);
                double root = (-b - sqrt_disc) / (2.0f * a);

                if (!ray_t.surrounds(root))
                {
                    // int ans = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
                    root = (-b + std::sqrt(discriminant)) / (2.0f * a);
                    if (!ray_t.surrounds(root))
                    {
                        hitInfo = Miss();
                        return false;
                    }
                }

                ray_t.max = root;

                /*HitInfo hitInfo{};*/

                hitInfo.HitPoint = (Math::Vector3<float>)ray.Origin + (Math::Vector3<float>)ray.Direction * root;
                hitInfo.Normal = (hitInfo.HitPoint - Origin).Normalize();
                hitInfo.Material = Material;
                hitInfo.ObjectID = id;

                return true;
            }

            std::shared_ptr<Materials::Material> Material;
        };
    }
}