#pragma once
#include "StandardIncludes.hpp"
#include "VK_Types.hpp"
#include "VK_Initializers.hpp"
#include "utils/Utilities.hpp"

class PipelineBuilder
{
public:
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineLayoutCreateInfo _pipelineLayoutCreateInfo;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;
    VkPipelineRenderingCreateInfo _renderInfo;
    VkFormat _colorAttachmentformat;
    
    PipelineBuilder() { clear(); }  

    void clear();

    VkPipeline build_pipeline(VkDevice device, Quanta_ImplVulkanH_RenderContext& context);

    void set_shaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
    void set_input_topology(VkPrimitiveTopology topology);
    void set_raster_defaults();
    void set_polygon_mode(VkPolygonMode mode);
    void set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace);
    void set_multisampling_none();
    void disable_blending();
    void enable_blending_additive();
    void enable_blending_alphablend();

    void set_color_attachment_format(VkFormat format);
    void set_depth_format(VkFormat format);
    void disable_depthtest();
    void enable_depthtest(bool depthWriteEnable, VkCompareOp op);
};

namespace vkutil
{
    bool load_shader_module(const std::string& filePath, VkDevice device, VkShaderModule &outShaderModule);
}