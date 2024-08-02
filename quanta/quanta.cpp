#include "quanta.hpp"


static uint32_t ConvertToRGBA(const Math::Vector3<float>& color)
{
    uint8_t r = (color.x * 255.0f);
    uint8_t g = (color.y * 255.0f);
    uint8_t b = (color.z * 255.0f);
    uint8_t a = (255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}

uint32_t DoMath::checkRay(float image_x, float image_y)
{
    // convert to 0,0 in the centre
    float coordX = image_x * 2.0f - 1.0f;
    float coordY = image_y * 2.0f - 1.0f;

    Math::Vector3<float> rayOrigin = Math::Vector3<float>(0.0f, 0.0f, 2.0f);
    Math::Vector3<float> rayDir = Math::Vector3<float>(coordX, coordY, -1.0f);

    float r = 0.5;

    float a = Math::LengthSquared(rayDir);
    float b = 2.0f * Math::Dot(rayOrigin, rayDir); 
    float c = Math::LengthSquared(rayOrigin) - Math::Sqr(r);

    // int ans = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
    float discriminant = Math::Sqr(b) - 4.0f * a * c;

    if (discriminant < 0.0f)
        return BLACK;

    float t0 = (-b + std::sqrt(discriminant)) / (2.0f * a);
    float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);

    Math::Vector3<float> hitPoint = rayOrigin + rayDir * t1;

    Math::Vector3<float> normal = Math::Normalize(hitPoint);

    Math::Vector3<float> res = Math::Clamp(normal, 0.0f, 1.0f);

    return ConvertToRGBA(res);
}
