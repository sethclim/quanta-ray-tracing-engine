#pragma once

#include <vector>

#include "../app/src/InputManager.hpp"
#include "../render_data/render_data.hpp"

#include <imgui.h>
#include "ImGuiHelpers.hpp"

//#include "InputManager.hpp"
#include "Application.hpp"


// TODO Don't I need these??
// #define IMGUI_IMPL_VULKAN_SHADER
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace QuantaEditor::Input
{
    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        // std::cout << "XPos " << xpos << " ypos " << ypos << std::endl;
        Core::Input::InputManager::GetInstance().SetPosition(xpos, ypos);
    }

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        Core::Input::InputManager &inputManager = Core::Input::InputManager::GetInstance();
        glm::vec2 mouse = inputManager.GetMousePosition();
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            inputManager.QueueEvent({mouse.x, mouse.y, GLFW_MOUSE_BUTTON_LEFT});
    }
}

class Editor : public Application
{

    // const std::vector<Vertex> vertices = {
    //     {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    //     {{0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    //     {{0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    //     {{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

    const std::vector<uint16_t>
        indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

public:
    Editor();
    void RenderLoop(double t, double fpsLimit) override;

private:
    void RenderUI();
    void OnRecordCommandBuffer(VkCommandBuffer cmd);

    void init_imgui();

    /* std::vector<int> CalculateLayout(int width, int height);
     DrawData RenderEditor();

     std::vector<int> dimensions;*/
};
