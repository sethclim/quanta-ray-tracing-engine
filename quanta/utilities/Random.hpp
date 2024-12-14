#pragma once
#include <cstdlib>

#include "Vector3.hpp"

namespace Utilities
{
    namespace Random
    {
        inline double random_double()
        {
            // Returns a random real in [0,1).
            return std::rand() / (RAND_MAX + 1.0);
        }

        inline double random_double(double min, double max)
        {
            // Returns a random real in [min,max).
            return min + (max - min) * random_double();
        }

        inline Math::Vector3<float> random_vec3()
        {
            return Math::Vector3<float>(random_double(), random_double(), random_double());
        }

        inline Math::Vector3<float> random_vec3(double min, double max)
        {
            return Math::Vector3<float>(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        inline Math::Vector3<float> random_unit_vector()
        {
            while (true)
            {
                auto p = random_vec3(-1, 1);
                auto lensq = LengthSquared(p);
                if (1e-160 < lensq && lensq <= 1)
                    return p / sqrt(lensq);
            }
        }

        inline Math::Vector3<float> Random_On_Hemisphere(const Math::Vector3<float> &normal)
        {
            Math::Vector3<float> on_unit_sphere = random_unit_vector();
            if (Dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
                return on_unit_sphere;
            else
                return Math::Vector3<float>(0, 0, 0) - on_unit_sphere;
        }
    } // namespace Random
} // namespace Utilities
