#include "quanta.hpp"

int DoMath::add(int a, int b)
{
    return a + b;
}


uint32_t DoMath::checkRay(int image_x, int image_y)
{
    //convert to 0,0 in the centre
    float x = image_x - ((float)m_width / 2);
    float y = image_y - ((float)m_height / 2);
    float z = 10;

    int r = 9;

    int cameraX = 0;
    int cameraY = 0;
    int cameraZ = -10;

    int rayDirX = cameraX - x;
    int rayDirY = cameraY - y;
    int rayDirZ = cameraZ - z;

    int a = (rayDirX * rayDirX) + (rayDirY * rayDirY) + (rayDirZ * rayDirZ);
    int b = (2 * cameraX * rayDirX) + (2 * cameraY * rayDirY) + (2 * cameraZ * rayDirZ);
    float c = (cameraX * cameraX) + (cameraY * cameraY) + (cameraZ * cameraZ) - (r * r);

    //int ans = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);
    float discriminant = (b * b) - (4 * a * c);

    //if intersects draw red
    if (discriminant >= 0)
    {
        return RED;
    }
    else
    {
        return BLACK;
    }
}
