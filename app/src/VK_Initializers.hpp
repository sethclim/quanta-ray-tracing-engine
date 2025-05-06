#ifndef VKINITIALIZERS_H
#define VKINITIALIZERS_H

#include <vulkan/vulkan.h>

namespace vkinit {

VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage,
    VkShaderModule shaderModule,
    const char* entry = "main");

VkRenderingAttachmentInfo attachment_info(
    VkImageView view, VkClearValue* clear, VkImageLayout layout /*= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL*/);

VkRenderingInfo rendering_info(VkExtent2D renderExtent, VkRenderingAttachmentInfo* colorAttachment,
    VkRenderingAttachmentInfo* depthAttachment);

} // namespace vkinit
#endif