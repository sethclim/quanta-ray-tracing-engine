#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Singleton.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Resolution.hpp"
#include "StandardIncludes.hpp"
#include "VulkanBackend.hpp"

class WindowController : public Singleton<WindowController>
{
public:
    WindowController();
    virtual ~WindowController();

    GLFWwindow *GetWindow()
    {
        return m_WindowHandle;
    }

    void NewWindow();
    Resolution GetResolution();
    glm::vec2 GetSize();

private:
    GLFWwindow *m_WindowHandle;

    void UpdateFrameBufferResized();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto windowController = reinterpret_cast<WindowController*>(glfwGetWindowUserPointer(window));
        windowController->UpdateFrameBufferResized();
    }
};

#endif