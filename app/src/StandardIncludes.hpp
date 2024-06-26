#ifndef STANDARDINCLUDES_H
#define STANDARDINCLUDES_H
#include <iostream>



#define M_ASSERT(_cond, _msg)     \
    if (!(_cond))                 \
    {                             \
        std::cout<< _msg << std::endl; \
        std::abort();             \
        glfwTerminate();          \
    }


#endif // !STANDARDINCLUDES_H