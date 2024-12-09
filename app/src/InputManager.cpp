#include "InputManager.hpp"


namespace Input
{

    static InputManager* s_Instance = nullptr;

    void InputManager::SetPosition(double x, double y)
    {
        xpos = x;
        ypos = y;
    }

    glm::vec2 InputManager::GetMousePosition()
    {
        return { xpos, ypos };
    }

    InputManager::InputManager()
    {
        xpos = 0;
        ypos = 0;
    };

    InputManager::~InputManager()
    {

    };

    InputManager& InputManager::GetInstance()
    {
        assert(s_Instance);
        return *s_Instance;
    };

    void InputManager::Init()
    {
        assert(!s_Instance);

        s_Instance = new InputManager();
    };

    void InputManager::ShutDown()
    {
        delete s_Instance;
        s_Instance = nullptr;
    };
}
       