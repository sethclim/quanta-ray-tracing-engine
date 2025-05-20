#pragma once

#include <cstdlib>

#include "Vector3.hpp"

namespace Utilities
{
    namespace Random
    {
        inline double Random_Double()
        {
            // Returns a random real in [0,1).
            return std::rand() / (RAND_MAX + 1.0);
        }

        inline double Random_Double(double min, double max)
        {
            // Returns a random real in [min,max)
            return min + (max - min) * Random_Double();
        }

        inline Math::Vector3<float> Random_Vec3()
        {
            return Math::Vector3<float>(Random_Double(), Random_Double(), Random_Double());
        }

        inline Math::Vector3<float> Random_Vec3(double min, double max)
        {
            return Math::Vector3<float>(Random_Double(min, max), Random_Double(min, max), Random_Double(min, max));
        }

        inline Math::Vector3<float> Random_Unit_Vector()
        {
            while (true)
            {
                auto p = Random_Vec3(-1, 1);
                auto lensq = p.LengthSquared();
                if (1e-160 < lensq && lensq <= 1)
                    return p / sqrt(lensq);
            }
        }

        inline Math::Vector3<float> Random_On_Hemisphere(const Math::Vector3<float> &normal)
        {
            Math::Vector3<float> on_unit_sphere = Random_Unit_Vector();
            if (on_unit_sphere.Dot(normal) > 0.0) // In the same hemisphere as the normal
                return on_unit_sphere;
            else
                return Math::Vector3<float>(0, 0, 0) - on_unit_sphere;
        }
    } // namespace Random
} // namespace Utilities
