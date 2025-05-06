
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <queue>

namespace Input
{
    struct MouseEvent {
        float x, y;
        float button;
    };

    class InputManager
    {
    public:
        static InputManager &GetInstance();
        static void Init();
        static void ShutDown();

        InputManager(const InputManager &) = delete;
        InputManager &operator=(const InputManager &) = delete;

        void SetPosition(double x, double y);
        glm::vec2 GetMousePosition();

        void AddCallback(std::function<void(const MouseEvent&)> callback);
        void QueueEvent(const MouseEvent& event);
        void ProcessEvents();

    private:
        InputManager();
        ~InputManager();


    private:
        double xpos;
        double ypos;

        std::vector<std::function<void(const MouseEvent&)>> callbacks;
        std::queue<MouseEvent> eventQueue;
    };

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        // std::cout << "XPos " << xpos << " ypos " << ypos << std::endl;
        InputManager::GetInstance().SetPosition(xpos, ypos);
    }

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        InputManager& inputManager = InputManager::GetInstance();
        glm::vec2 mouse = inputManager.GetMousePosition();
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            inputManager.QueueEvent({ mouse.x, mouse.y, GLFW_MOUSE_BUTTON_LEFT });
    }

}

#endif // INPUT_MANAGER_H
