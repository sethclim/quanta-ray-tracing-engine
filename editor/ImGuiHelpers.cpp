
#include "ImGuiHelpers.hpp"

namespace ImGuiH
{
    //--------------------------------------------------------------------------------------------------
//
// If GLFW has been initialized, returns the DPI scale of the primary monitor. Otherwise, returns 1.
//
    float getDPIScale()
    {
        // Cached DPI scale, so that this doesn't change after the first time code calls getDPIScale.
        // A negative value indicates that the value hasn't been computed yet.
        static float cached_dpi_scale = -1.0f;

        if (cached_dpi_scale < 0.0f)
        {
            // Compute the product of the monitor DPI scale and any DPI scale
            // set in the NVPRO_DPI_SCALE variable.
            cached_dpi_scale = 1.0f;

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            assert(monitor);
            if (monitor != nullptr)
            {
                float y_scale;
                glfwGetMonitorContentScale(monitor, &cached_dpi_scale, &y_scale);
            }
            // Otherwise, GLFW isn't initialized yet, but might be in the future.
            // (Note that this code assumes all samples use GLFW.)

            // Multiply by the value of the NVPRO_DPI_SCALE environment variable.
            const char* dpi_env = getenv("NVPRO_DPI_SCALE");
            if (dpi_env)
            {
                const float parsed_dpi_env = strtof(dpi_env, nullptr);
                if (parsed_dpi_env != 0.0f)
                {
                    cached_dpi_scale *= parsed_dpi_env;
                }
            }

            cached_dpi_scale = (cached_dpi_scale > 0.0f ? cached_dpi_scale : 1.0f);
        }

        return cached_dpi_scale;
    }

    // Static member declaration
    ImGuiID Panel::dockspaceID{ 0 };

    void Panel::Begin(Side side /*= Side::Right*/, float alpha /*= 0.5f*/, char* name /*= nullptr*/)
    {
        // Keeping the unique ID of the dock space
        dockspaceID = ImGui::GetID("DockSpace");

        // The dock need a dummy window covering the entire viewport.
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // All flags to dummy window
        ImGuiWindowFlags host_window_flags{};
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        host_window_flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground;

        // Starting dummy window
        char label[32];
        ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin(label, nullptr, host_window_flags);
        ImGui::PopStyleVar(3);

        // The central node is transparent, so that when UI is draw after, the image is visible
        // Auto Hide Bar, no title of the panel
        // Center is not dockable, that is for the scene
        ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar
            | ImGuiDockNodeFlags_NoDockingOverCentralNode;

        // Default panel/window is name setting
        std::string dock_name("Settings");
        if (name != nullptr)
            dock_name = name;

        // Building the splitting of the dock space is done only once
        if (!ImGui::DockBuilderGetNode(dockspaceID))
        {
            ImGui::DockBuilderRemoveNode(dockspaceID);
            ImGui::DockBuilderAddNode(dockspaceID, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

            ImGuiID dock_main_id = dockspaceID;

            // Slitting all 4 directions, targetting (320 pixel * DPI) panel width, (180 pixel * DPI) panel height.
            const float xRatio = glm::clamp<float>(320.0f * getDPIScale() / viewport->WorkSize[0], 0.01f, 0.499f);
            const float yRatio = glm::clamp<float>(180.0f * getDPIScale() / viewport->WorkSize[1], 0.01f, 0.499f);
            ImGuiID     id_left, id_right, id_up, id_down;

            // Note, for right, down panels, we use the n / (1 - n) formula to correctly split the space remaining from the left, up panels.
            id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, xRatio, nullptr, &dock_main_id);
            id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, xRatio / (1 - xRatio), nullptr, &dock_main_id);
            id_up = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, yRatio, nullptr, &dock_main_id);
            id_down = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, yRatio / (1 - yRatio), nullptr, &dock_main_id);

            ImGui::DockBuilderDockWindow(side == Side::Left ? dock_name.c_str() : "Dock_left", id_left);
            ImGui::DockBuilderDockWindow(side == Side::Right ? dock_name.c_str() : "Dock_right", id_right);
            ImGui::DockBuilderDockWindow("Dock_up", id_up);
            ImGui::DockBuilderDockWindow("Dock_down", id_down);
            ImGui::DockBuilderDockWindow("Scene", dock_main_id);  // Center

            ImGui::DockBuilderFinish(dock_main_id);
        }

        // Setting the panel to blend with alpha
        ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(col.x, col.y, col.z, alpha));

        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
        ImGui::PopStyleColor();
        ImGui::End();

        // The panel
        if (alpha < 1)
            ImGui::SetNextWindowBgAlpha(alpha);  // For when the panel becomes a floating window
        ImGui::Begin(dock_name.c_str());
    }

    //--------------------------------------------------------------------------------------------------
    // Setting a dark style for the GUI
    // The colors were coded in sRGB color space, set the useLinearColor
    // flag to convert to linear color space.
    void setStyle(bool useLinearColor)
    {
        typedef ImVec4(*srgbFunction)(float, float, float, float);
        srgbFunction   passthrough = [](float r, float g, float b, float a) -> ImVec4 { return ImVec4(r, g, b, a); };
        srgbFunction   toLinear = [](float r, float g, float b, float a) -> ImVec4 {
            auto toLinearScalar = [](float u) -> float {
                return u <= 0.04045 ? 25 * u / 323.f : powf((200 * u + 11) / 211.f, 2.4f);
            };
            return ImVec4(toLinearScalar(r), toLinearScalar(g), toLinearScalar(b), a);
        };
        srgbFunction srgb = useLinearColor ? toLinear : passthrough;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.WindowBorderSize = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.FrameRounding = 2.0f;
        style.FrameBorderSize = 1.0f;
        style.GrabRounding = 4.0f;
        style.IndentSpacing = 12.0f;
        style.Colors[ImGuiCol_WindowBg] = srgb(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg] = srgb(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg] = srgb(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_PopupBg] = srgb(0.135f, 0.135f, 0.135f, 1.0f);
        style.Colors[ImGuiCol_Border] = srgb(0.4f, 0.4f, 0.4f, 0.5f);
        style.Colors[ImGuiCol_FrameBg] = srgb(0.05f, 0.05f, 0.05f, 0.5f);

        // Normal
        ImVec4                normal_color = srgb(0.465f, 0.465f, 0.525f, 1.0f);
        std::vector<ImGuiCol> to_change_nrm;
        to_change_nrm.push_back(ImGuiCol_Header);
        to_change_nrm.push_back(ImGuiCol_SliderGrab);
        to_change_nrm.push_back(ImGuiCol_Button);
        to_change_nrm.push_back(ImGuiCol_CheckMark);
        to_change_nrm.push_back(ImGuiCol_ResizeGrip);
        to_change_nrm.push_back(ImGuiCol_TextSelectedBg);
        to_change_nrm.push_back(ImGuiCol_Separator);
        to_change_nrm.push_back(ImGuiCol_FrameBgActive);
        for (auto c : to_change_nrm)
        {
            style.Colors[c] = normal_color;
        }

        // Active
        ImVec4                active_color = srgb(0.365f, 0.365f, 0.425f, 1.0f);
        std::vector<ImGuiCol> to_change_act;
        to_change_act.push_back(ImGuiCol_HeaderActive);
        to_change_act.push_back(ImGuiCol_SliderGrabActive);
        to_change_act.push_back(ImGuiCol_ButtonActive);
        to_change_act.push_back(ImGuiCol_ResizeGripActive);
        to_change_act.push_back(ImGuiCol_SeparatorActive);
        for (auto c : to_change_act)
        {
            style.Colors[c] = active_color;
        }

        // Hovered
        ImVec4                hovered_color = srgb(0.565f, 0.565f, 0.625f, 1.0f);
        std::vector<ImGuiCol> to_change_hover;
        to_change_hover.push_back(ImGuiCol_HeaderHovered);
        to_change_hover.push_back(ImGuiCol_ButtonHovered);
        to_change_hover.push_back(ImGuiCol_FrameBgHovered);
        to_change_hover.push_back(ImGuiCol_ResizeGripHovered);
        to_change_hover.push_back(ImGuiCol_SeparatorHovered);
        for (auto c : to_change_hover)
        {
            style.Colors[c] = hovered_color;
        }


        style.Colors[ImGuiCol_TitleBgActive] = srgb(0.465f, 0.465f, 0.465f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = srgb(0.125f, 0.125f, 0.125f, 1.0f);
        style.Colors[ImGuiCol_Tab] = srgb(0.05f, 0.05f, 0.05f, 0.5f);
        style.Colors[ImGuiCol_TabHovered] = srgb(0.465f, 0.495f, 0.525f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = srgb(0.282f, 0.290f, 0.302f, 1.0f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = srgb(0.465f, 0.465f, 0.465f, 0.350f);

        //Colors_ext[ImGuiColExt_Warning] = srgb (1.0f, 0.43f, 0.35f, 1.0f);

        ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueWheel);
    }


}