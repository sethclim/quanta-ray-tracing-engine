#pragma once

#include <tgmath.h>
#include "vector3.hpp"

namespace Utilities
{
    namespace Color
    {
        static uint32_t ConvertToRGBA(const Math::Vector4<float> &color)
        {
            uint8_t r = (color.x * 255.0f);
            uint8_t g = (color.y * 255.0f);
            uint8_t b = (color.z * 255.0f);
            uint8_t a = (color.w * 255.0f);

            return (a << 24) | (b << 16) | (g << 8) | r;
        }

        inline double linear_to_gamma(double linear_component)
        {
            if (linear_component > 0)
                return std::sqrt(linear_component);

            return 0;
        }

    }
}