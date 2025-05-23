#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Utils
{

    static uint32_t GetVulkanMemoryType(VkMemoryPropertyFlags properties, uint32_t type_bits, VkPhysicalDevice physicalDevice)
    {
        VkPhysicalDeviceMemoryProperties prop;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &prop);
        for (uint32_t i = 0; i < prop.memoryTypeCount; i++)
        {
            if ((prop.memoryTypes[i].propertyFlags & properties) == properties && type_bits & (1 << i))
                return i;
        }

        return 0xffffffff;
    }

    static uint32_t BytesPerPixel(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::RGBA:
            return 4;
        case ImageFormat::RGBA32F:
            return 16;
        }
        return 0;
    }

    static VkFormat WalnutFormatToVulkanFormat(ImageFormat format)
    {
        switch (format)
        {
        case ImageFormat::RGBA:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case ImageFormat::RGBA32F:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        return (VkFormat)0;
    }

}

Image::Image(std::string_view path, VulkanBackend& vkBackend)
    : m_Filepath(path), m_VKBackend(vkBackend)
{
    int width, height, channels;
    uint8_t *data = nullptr;

    if (stbi_is_hdr(m_Filepath.c_str()))
    {
        data = (uint8_t *)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 4);
        m_Format = ImageFormat::RGBA32F;
    }
    else
    {
        data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 4);
        m_Format = ImageFormat::RGBA;
    }

    m_Width = width;
    m_Height = height;

    AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
    SetData(vkBackend, data);
    stbi_image_free(data);
}

Image::Image(uint32_t width, uint32_t height, ImageFormat format, VulkanBackend& vkBackend, const void* data)
    : m_Width(width), m_Height(height), m_Format(format), m_VKBackend(vkBackend)
{
    AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
    if (data)
        SetData(vkBackend, data);
}

Image::~Image()
{
    Release();
}

void Image::AllocateMemory( uint64_t size)
{
    VkFormat vulkanFormat = Utils::WalnutFormatToVulkanFormat(m_Format);

    createImage(vulkanFormat,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_ImageView = m_VKBackend.createImageView(m_Image, vulkanFormat);
    createTextureSampler();
    m_VKBackend.createDescriptorSets(m_Sampler, m_ImageView);
}

void Image::Release()
{
    // TODO - find out what this does
    // Application::SubmitResourceFree([sampler = m_Sampler, imageView = m_ImageView, image = m_Image,
    //                                  memory = m_Memory, stagingBuffer = m_StagingBuffer, stagingBufferMemory = m_StagingBufferMemory]()
    //                                 {
    VkDevice &device = m_VKBackend.GetDevice();

    vkDestroySampler(device, m_Sampler, nullptr);
    vkDestroyImageView(device, m_ImageView, nullptr);
    vkDestroyImage(device, m_Image, nullptr);
    vkFreeMemory(device, m_Memory, nullptr);
    vkDestroyBuffer(device, m_StagingBuffer, nullptr);
    vkFreeMemory(device, m_StagingBufferMemory, nullptr);

    m_Sampler = nullptr;
    m_ImageView = nullptr;
    m_Image = nullptr;
    m_Memory = nullptr;
    m_StagingBuffer = nullptr;
    m_StagingBufferMemory = nullptr;
}

void Image::SetData(VulkanBackend vkBackend, const void *pixels)
{
    VkDevice &device = vkBackend.GetDevice();

    size_t upload_size = m_Width * m_Height * Utils::BytesPerPixel(m_Format);

    VkResult err;

    if (!m_StagingBuffer)
    {
        vkBackend.createBuffer(upload_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_StagingBuffer, m_StagingBufferMemory);
    }

    // Upload to Buffer
    {

        void *data;
        vkMapMemory(device, m_StagingBufferMemory, 0, upload_size, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(upload_size));
        vkUnmapMemory(device, m_StagingBufferMemory);
    }

    transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage();
    transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Image::Resize(VulkanBackend vkBackend, uint32_t width, uint32_t height)
{
    if (m_Image && m_Width == width && m_Height == height)
        return;

    // TODO: max size?

    m_Width = width;
    m_Height = height;

    Release();
    AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
}

void Image::createImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = m_Width;
    imageInfo.extent.height = m_Height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_VKBackend.GetDevice(), &imageInfo, nullptr, &m_Image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_VKBackend.GetDevice(), m_Image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = m_VKBackend.findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_VKBackend.GetDevice(), &allocInfo, nullptr, &m_Memory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(m_VKBackend.GetDevice(), m_Image, m_Memory, 0);
}

void Image::copyBufferToImage()
{
    VkCommandBuffer commandBuffer = m_VKBackend.beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        m_Width,
        m_Height,
        1};

    vkCmdCopyBufferToImage(commandBuffer, m_StagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    m_VKBackend.endSingleTimeCommands(commandBuffer);
}

void Image::transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = m_VKBackend.beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_Image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    m_VKBackend.endSingleTimeCommands(commandBuffer);
}

void Image::createTextureSampler()
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(m_VKBackend.GetPhysicalDevice(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(m_VKBackend.GetDevice(), &samplerInfo, nullptr, &m_Sampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}