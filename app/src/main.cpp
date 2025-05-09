// #include <iostream>

// #include "application.hpp"

#include "SceneManager.hpp"

int main()
{
    // auto app = Application();
    // app.Run();

    auto sm = SceneManager();
    sm.SaveScene();
    return 0;
}