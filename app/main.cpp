// Your First C++ Program

#include <iostream>
#include "quanta.hpp"

int main()
{
    auto math = DoMath();
    auto res = math.add(1, 2);
    std::cout << "Hello World! " << res << std::endl;
    return 0;
}