
#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include "vulkan/vulkan.h"
#include "VulkanBackend.hpp"

enum class ImageFormat
{
    None = 0,
    RGBA,
    RGBA32F
};

class Image
{
public:
    Image(std::string_view path, VulkanBackend &vkBackend);
    Image(uint32_t width, uint32_t height, ImageFormat format, VulkanBackend &vkBackend, const void *data = nullptr);
    ~Image();

    void SetData(VulkanBackend vkBackend, const void *data);

    VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }

    void Resize(VulkanBackend vkBackend, uint32_t width, uint32_t height);

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }

private:
    void AllocateMemory(uint64_t size);
    void Release();

    void createImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
    void copyBufferToImage();
    void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
    void createTextureSampler();

private:
    uint32_t m_Width = 0, m_Height = 0;

    VkImage m_Image = nullptr;
    VkImageView m_ImageView = nullptr;
    VkDeviceMemory m_Memory = nullptr;
    VkSampler m_Sampler = nullptr;

    ImageFormat m_Format = ImageFormat::None;

    VkBuffer m_StagingBuffer = nullptr;
    VkDeviceMemory m_StagingBufferMemory = nullptr;

    size_t m_AlignedSize = 0;

    VkDescriptorSet m_DescriptorSet = nullptr;

    std::string m_Filepath;

    VulkanBackend &m_VKBackend;
};

#endif