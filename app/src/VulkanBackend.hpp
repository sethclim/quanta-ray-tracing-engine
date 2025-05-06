#ifndef VULKANBACKEND_H
#define VULKANBACKEND_H

#include "quanta.hpp"

#include <filesystem>

// #define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

#include "StandardIncludes.hpp"
#include "WindowController.hpp"
#include "VK_Pipeline.hpp"
#include "VK_Types.hpp"

#include "imgui.h"
#include <imconfig.h>
#define IMGUI_IMPL_VULKAN_SHADER
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "ImGUI.hpp"

//#include "VK_Images.hpp"

const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#include "Random.hpp"

class Image;

//
//
// struct DrawData
//{
//    std::vector<Vertex> vertices;
//    std::vector<uint16_t> indices;
//};

// const std::vector<Vertex> vertices = {
//     {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//     {{1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//     {{1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//     {{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

// const std::vector<uint16_t> indices = {
//     0, 1, 2, 2, 3, 0};

class VulkanBackend : public Singleton<VulkanBackend>
{
public:
    void SetupVulkan(const char **extensions, uint32_t extensions_count, const std::vector<Vertex> vertices, const std::vector<uint16_t> indices, int debugBufferSize, bool debug);

    void drawFrame(const std::vector<uint16_t> indices, int debugLinesCount);
    VkPhysicalDevice &GetPhysicalDevice();
    VkDevice &GetDevice();
    void createDescriptorSets(VkSampler &sampler, VkImageView &image_view);
    VkImageView createImageView(VkImage &image, VkFormat format);

    void SetFrameBufferResized(bool resized) { framebufferResized = resized; }

    Quanta_ImplVulkanH_RenderContext &GetRenderContext() { return context; }

private:
    void recreateSwapChain();
    void cleanupSwapChain();

    void createInstance(const char **glfwExtensions, uint32_t extensions_count);

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createDebugPipeline();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, const std::vector<uint16_t> indices, int debugLinesCount);

    void createSyncObjects();
    void updateUniformBuffer(uint32_t currentImage);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, int width, int height);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR &surface);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR &surface);

    void createVertexBuffer(const std::vector<Vertex> vertices);

    void createIndexBuffer(const std::vector<uint16_t> indices);

    void createUniformBuffers();

    void createDescriptorPool();

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createDebugBuffer(VkDevice device, VkDeviceSize bufferSize);

    void drawDebugRays(VkCommandBuffer commandBuffer, int numLines);

    void init_imgui();

    void draw_imgui(VkCommandBuffer cmd);

public:
    void updateDebugBuffer(std::vector<Utilities::DebugLine> &newLines);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
    bool checkValidationLayerSupport();

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR &surface);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    std::vector<char> readFile(const std::string &filename);

    VkShaderModule createShaderModule(const std::vector<char> &code);

private:
    VkAllocationCallbacks *g_Allocator = VK_NULL_HANDLE;
    VkInstance g_Instance = VK_NULL_HANDLE;
    VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice g_Device = VK_NULL_HANDLE;
    uint32_t g_QueueFamily = -1;
    VkQueue g_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue g_PresentQueue = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMapped;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D swapChainExtent = {0, 0};
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    bool framebufferResized = false;
     
    Quanta_ImplVulkanH_RenderContext context;

    // Debug
    VkBuffer debugVertexBuffer;
    VkDeviceMemory debugVertexBufferMemory;
    Utilities::DebugLine *mappedDebugMemory = nullptr; // Pointer to mapped memory

    int maxDebugLines;
    bool debug;

    //draw resources
    AllocatedImage _drawImage;
};

#endif