// Your First C++ Program

#include <iostream>
#include "application.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
    auto app = Application();
    app.Run();
    return 0;
}