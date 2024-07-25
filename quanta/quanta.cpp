#include "quanta.hpp"

int DoMath::add(int a, int b)
{
    return a + b;
}

uint32_t DoMath::checkRay(int image_x, int image_y)
{
    // convert to 0,0 in the centre
    int coordX = image_x - ((float)m_width / 2);
    int coordY = image_y - ((float)m_height / 2);

    Vector3<int> screenCoord3d = Vector3<int>(coordX, coordY, 10);

    int r = 9;

    Vector3<int> cameraPosition = Vector3<int>(0, 0, -10);

    Vector3<int> rayDir = cameraPosition - screenCoord3d;

    int a = (rayDir.x * rayDir.x) + (rayDir.y * rayDir.y) + (rayDir.z * rayDir.z);
    int b = (2 * cameraPosition.x * rayDir.x) + (2 * cameraPosition.y * rayDir.y) + (2 * cameraPosition.z * rayDir.z);
    float c = (cameraPosition.x * cameraPosition.x) + (cameraPosition.y * cameraPosition.y) + (cameraPosition.z * cameraPosition.z) - (r * r);

    // int ans = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
    float discriminant = (b * b) - (4 * a * c);

    // if intersects draw red
    if (discriminant >= 0)
    {
        return RED;
    }
    else
    {
        return BLACK;
    }
}
