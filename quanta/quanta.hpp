
#include <cstdint>
#include <tgmath.h>
#include "vector3.hpp"

const uint32_t RED = 0x000000ff;
const uint32_t BLUE = 0x00ff0000;
const uint32_t BLACK = 0x00000000;

class DoMath
{
public:
    DoMath(){}

    uint32_t checkRay(float x, float y);
};