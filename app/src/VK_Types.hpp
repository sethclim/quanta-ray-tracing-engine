
#ifndef VKTYPES_H
#define VKTYPES_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

#include "../../render_data/render_data.hpp"

#include <optional>
#include <cstddef>

namespace RenderData
{
    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

    struct DebugVertex
    {
        glm::vec3 position;
    };

    VkVertexInputBindingDescription getDebugBindingDescription();
    std::array<VkVertexInputAttributeDescription, 1> getDebugAttributeDescriptions();
}

struct Quanta_ImplVulkanH_RenderContext
{
    int Width;
    int Height;
    VkSwapchainKHR Swapchain;
    VkSurfaceKHR Surface;
    VkSurfaceFormatKHR SurfaceFormat;
    // VkPresentModeKHR    PresentMode;
    VkRenderPass RenderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline GraphicsPipeline; // The window pipeline may uses a different VkRenderPass than the one passed in ImGui_ImplVulkan_InitInfo
    VkPipelineLayout PipelineLayout;

    VkPipeline DebugPipeline; // Debug pipeline for drawing rays

    // bool                UseDynamicRendering;
    // bool                ClearEnable;
    // VkClearValue        ClearValue;
    // uint32_t            FrameIndex;             // Current frame being rendered to (0 <= FrameIndex < FrameInFlightCount)
    uint32_t ImageIndex; // Number of simultaneous in-flight frames (returned by vkGetSwapchainImagesKHR, usually derived from min_image_count)
    // uint32_t            SemaphoreCount;         // Number of simultaneous in-flight frames + 1, to be able to use it in vkAcquireNextImageKHR
    // uint32_t            SemaphoreIndex;         // Current set of swapchain wait semaphores we're using (needs to be distinct from per frame data)
    // ImGui_ImplVulkanH_Frame* Frames;
    // ImGui_ImplVulkanH_FrameSemaphores* FrameSemaphores;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    Quanta_ImplVulkanH_RenderContext()
    {
        memset((void *)this, 0, sizeof(*this));
        // PresentMode = (VkPresentModeKHR)~0;     // Ensure we get an error if user doesn't set this.
        // ClearEnable = true;
    }
};

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

#endif