#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include "Singleton.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Resolution.hpp"
#include "StandardIncludes.hpp"

class VulkanBackend;

namespace Window
{
    class WindowController
    {
    public:
        static WindowController &GetInstance();
        static void Init(VulkanBackend &vkBackend);
        static void ShutDown();

        WindowController(const WindowController &) = delete;
        WindowController &operator=(const WindowController &) = delete;

        GLFWwindow *GetWindow()
        {
            return m_WindowHandle;
        }

        void NewWindow();
        Resolution GetResolution();
        glm::vec2 GetSize();

    private:
        WindowController(VulkanBackend &vkBackend);
        ~WindowController();
        GLFWwindow *m_WindowHandle;

        void UpdateFrameBufferResized();

        static void framebufferResizeCallback(GLFWwindow *window, int width, int height)
        {
            auto windowController = reinterpret_cast<WindowController *>(glfwGetWindowUserPointer(window));
            windowController->UpdateFrameBufferResized();
        }

        VulkanBackend &m_VKBackend;
    };
}

#endif
