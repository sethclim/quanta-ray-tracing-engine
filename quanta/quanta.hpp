
#include <cstdint>
#include <tgmath.h>

const uint32_t RED = 0x000000ff;
const uint32_t BLUE = 0x00ff0000;
const uint32_t BLACK = 0x00000000;

class DoMath
{
public:



    DoMath(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    int add(int a, int b);


    uint32_t checkRay(int x, int y);

private:
    int m_width, m_height;
};