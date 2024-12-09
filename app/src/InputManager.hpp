
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>

namespace Input
{

    class InputManager
    {
    public:
        static InputManager& GetInstance();
        static void Init();
        static void ShutDown();

        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;

        void SetPosition(double x, double y);

    private:
        InputManager();
        ~InputManager();

    private:
        double xpos; 
        double ypos;
    };

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        std::cout << "XPos " << xpos << " ypos " << ypos << std::endl;
        InputManager::GetInstance().SetPosition(xpos, ypos);
    }
}

#endif // INPUT_MANAGER_H