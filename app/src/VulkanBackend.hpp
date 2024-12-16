#ifndef VULKANBACKEND_H
#define VULKANBACKEND_H

#include <filesystem>

// #define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

#include "StandardIncludes.hpp"
#include "WindowController.hpp"
#include "VK_Pipeline.hpp"

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

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
};
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

    void SetupVulkan(const char **extensions, uint32_t extensions_count, const std::vector<Vertex> vertices, const std::vector<uint16_t> indices);

    void drawFrame(const std::vector<uint16_t> indices);
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

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, const std::vector<uint16_t> indices);

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

public:
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
};

#endif