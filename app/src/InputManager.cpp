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

    void InputManager::AddCallback(std::function<void(const MouseEvent&)> callback) {
        callbacks.push_back(callback);
    }

    void InputManager::QueueEvent(const MouseEvent& event) {
        eventQueue.push(event);
    }

    void InputManager::ProcessEvents() {
        while (!eventQueue.empty()) {
            MouseEvent event = eventQueue.front();
            eventQueue.pop();
            for (auto& callback : callbacks) {
                callback(event);
            }
        }
    }
}
       