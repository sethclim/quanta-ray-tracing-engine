#pragma once

#include <tgmath.h>
#include "vector3.hpp"

namespace Utilities
{
    namespace Color
    {
        static uint32_t ConvertToRGBA(float _r, float _g, float _b, float _a)
        {
            uint8_t r = (_r * 255.0f);
            uint8_t g = (_g * 255.0f);
            uint8_t b = (_b * 255.0f);
            uint8_t a = (_a * 255.0f);

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