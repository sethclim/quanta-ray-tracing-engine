#include "Application.hpp"

class Runtime : public Application
{
public:
    Runtime();

private:
    const std::vector<uint16_t>
        indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
};
