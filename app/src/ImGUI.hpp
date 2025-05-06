#ifndef IMGUI_H
#define IMGUI_H

#include <stdexcept>

#include <vulkan/vulkan.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_vulkan.h"

#include <GLFW/glfw3.h>

#include "VK_Types.hpp"
#include "VulkanBackend.hpp"

namespace ImGUI
{
    //void CreateImGUIRenderPass(VkDevice g_Device, Quanta_ImplVulkanH_RenderContext context, VkFormat swapChainImageFormat)
    //{
    //    VkAttachmentDescription attachment = {};

    //    attachment.format = swapChainImageFormat;
    //    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    //    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    //    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    //    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    //    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    //    attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //    VkAttachmentReference color_attachment = {};
    //    color_attachment.attachment = 0;
    //    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    //    VkSubpassDescription subpass = {};
    //    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //    subpass.colorAttachmentCount = 1;
    //    subpass.pColorAttachments = &color_attachment;

    //    VkSubpassDependency dependency = {};
    //    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    //    dependency.dstSubpass = 0;
    //    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    //    dependency.srcAccessMask = 0; // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    //    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    //    VkRenderPassCreateInfo info = {};
    //    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    //    info.attachmentCount = 1;
    //    info.pAttachments = &attachment;
    //    info.subpassCount = 1;
    //    info.pSubpasses = &subpass;
    //    info.dependencyCount = 1;
    //    info.pDependencies = &dependency;

    //    if (vkCreateRenderPass(g_Device, &info, nullptr, &context.RenderPass) != VK_SUCCESS)
    //    {
    //        throw std::runtime_error("failed to create ImGUI render pass!");
    //    }
    //}

   /* void UploadImGUIFonts()
    {
        VkCommandBuffer commandBuffer = VulkanBackend::GetInstance().beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture();
        VulkanBackend::GetInstance().endSingleTimeCommands(commandBuffer);
    }*/

    //--------------------------------------------------------------------------------------------------
    //
    // If GLFW has been initialized, returns the DPI scale of the primary monitor. Otherwise, returns 1.
    //
    float getDPIScale();

    //--------------------------------------------------------------------------------------------------
    // Creating a window panel
    // - Panel will be on the left or the right side of the window.
    // - It fills the height of the window, and stays on the side it was created.
    //
    class Panel /*static*/
    {
        static ImGuiID dockspaceID;

    public:
        // Side where the panel will be
        enum class Side
        {
            Left,
            Right,
        };


        // Starting the panel, equivalent to ImGui::Begin for a window. Need ImGui::end()
        static void Begin(Side side = Side::Right, float alpha = 0.5f, char* name = nullptr);

        // Mirror begin but can use directly End()
        static void End() { ImGui::End(); }

        // Return the position and size of the central display
        static void CentralDimension(ImVec2& pos, ImVec2& size)
        {
            auto dock_main = ImGui::DockBuilderGetCentralNode(dockspaceID);
            if (dock_main)
            {
                pos = dock_main->Pos;
                size = dock_main->Size;
            }
        }
    };
}

#endif