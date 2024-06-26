#include "Singleton.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Resolution.hpp"
#include "StandardIncludes.hpp"

class WindowController : public Singleton<WindowController>
{
public:
    WindowController();
    virtual ~WindowController();

    GLFWwindow *GetWindow()
    {
        if (m_Window == nullptr)
        {
            NewWindow();
        }
        return m_Window;
    }

    void NewWindow();
    Resolution GetResolution();
    glm::vec2 GetSize();

private:
    GLFWwindow *m_Window;
};