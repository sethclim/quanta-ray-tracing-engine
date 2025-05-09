#include "VK_Pipeline.hpp"


void PipelineBuilder::clear()
{
    // clear all of the structs we need back to 0 with their correct stype

    _inputAssembly = {};
    _inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    _inputAssembly.pNext = nullptr;
    _inputAssembly.flags = 0;
    _inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    _inputAssembly.primitiveRestartEnable = VK_FALSE;

    _rasterizer = {};
    _rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    _rasterizer.flags = 0;
    _rasterizer.depthClampEnable = VK_FALSE;
    _rasterizer.rasterizerDiscardEnable = VK_FALSE;
    _rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    _rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    _rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    _rasterizer.depthBiasEnable = VK_FALSE;
    _rasterizer.depthBiasConstantFactor = 0.0f;
    _rasterizer.depthBiasClamp = 0.0f;
    _rasterizer.depthBiasSlopeFactor = 0.0f;
    _rasterizer.lineWidth = 1.0f;

    _colorBlendAttachment = {};
    _colorBlendAttachment.blendEnable = VK_FALSE;
    _colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    _colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    _colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    _colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    _colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    _multisampling = {};
    _multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    _multisampling.pNext = nullptr;
    _multisampling.flags = 0;
    _multisampling.sampleShadingEnable = VK_FALSE;
    // multisampling defaulted to no multisampling (1 sample per pixel)
    _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    _multisampling.minSampleShading = 1.0f;
    _multisampling.pSampleMask = nullptr;
    // no alpha to coverage either
    _multisampling.alphaToCoverageEnable = VK_FALSE;
    _multisampling.alphaToOneEnable = VK_FALSE;

    _pipelineLayout = {};

    _depthStencil = {};
    _depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    _depthStencil.pNext = nullptr;
    _depthStencil.flags = 0;
    _depthStencil.depthTestEnable = VK_FALSE;
    _depthStencil.depthWriteEnable = VK_FALSE;
    _depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
    _depthStencil.depthBoundsTestEnable = VK_FALSE;
    _depthStencil.stencilTestEnable = VK_FALSE;
    _depthStencil.front = {};
    _depthStencil.back = {};
    _depthStencil.minDepthBounds = 0.f;
    _depthStencil.maxDepthBounds = 1.f;

    _renderInfo = {};
    _renderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    _renderInfo.pNext = nullptr;
    _renderInfo.viewMask = 0;                              // No multiview
    _renderInfo.colorAttachmentCount = 1;
    VkFormat colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
    _renderInfo.pColorAttachmentFormats = &colorFormat;
    _renderInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;           // No depth
    _renderInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;          // No stencil

    _pipelineLayoutCreateInfo = {};
    _pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    _pipelineLayoutCreateInfo.pNext = nullptr;
    _pipelineLayoutCreateInfo.setLayoutCount = 1;
    _pipelineLayoutCreateInfo.pSetLayouts = nullptr;
    _pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    _pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

    _shaderStages.clear();
}

//#pragma optimize("", off)

VkPipeline PipelineBuilder::build_pipeline(
    VkDevice device,
    Quanta_ImplVulkanH_RenderContext &context,
    std::optional<std::pair<VkVertexInputBindingDescription, std::array<VkVertexInputAttributeDescription, 1>>> customVertexInfoDescriptions)
{
    VkViewport defaultViewport = {};
    defaultViewport.x = 0.0f;
    defaultViewport.y = 0.0f;
    defaultViewport.width = 1.0f;  // or your default width
    defaultViewport.height = 1.0f; // or your default height
    defaultViewport.minDepth = 0.0f;
    defaultViewport.maxDepth = 1.0f;

    VkRect2D defaultScissor = {};
    defaultScissor.offset = { 0, 0 };
    defaultScissor.extent = { 1, 1 }; // or your default dimensions

    // make viewport state from our stored viewport and scissor.
    // at the moment we wont support multiple viewports or scissors
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr; //TODO for now this breaks it
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr;

    // setup dummy color blending. We arent using transparent objects yet
    // the blending is just "no blend", but we do write to the color attachment
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;
    colorBlending.flags = 0;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &_colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // ADD something to create this and call after set shaders IG
    //  completely clear VertexInputStateCreateInfo, as we have no need for it
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo = {};
    _vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    _vertexInputInfo.pNext = nullptr;
    _vertexInputInfo.flags = 0;

    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

    auto bindingDescription = RenderData::getBindingDescription();
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, color);

    _vertexInputInfo.vertexBindingDescriptionCount = 1;
    _vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    _vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    _vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    if (customVertexInfoDescriptions)
    {
        auto bindingDescription = customVertexInfoDescriptions->first;
        auto attributeDescriptions = customVertexInfoDescriptions->second;

        _vertexInputInfo.vertexBindingDescriptionCount = 1;
        _vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        _vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        _vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    }
    //else
    //{
    //    // auto attributeDescriptions = RenderData::getAttributeDescriptions();

    //    // pCreateInfos[0].pVertexInputState->pVertexAttributeDescriptions[2].format

    //    /*     std::cout << "attributeDescriptions[2].format " << attributeDescriptions[2].format << std::endl;*/

    //}

    printf("Context address = %p\n", (void*)&context);

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO; 
    dynamicInfo.pNext = nullptr;
    dynamicInfo.flags = 0;
    dynamicInfo.dynamicStateCount = 2;
    VkDynamicState state[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    dynamicInfo.pDynamicStates = &state[0];

    _pipelineLayoutCreateInfo.setLayoutCount = 1;
    printf("DescriptorSetLayout = %p\n", (void*)context.descriptorSetLayout);
    _pipelineLayoutCreateInfo.pSetLayouts = &context.descriptorSetLayout;

    assert(context.descriptorSetLayout != VK_NULL_HANDLE);

    if (vkCreatePipelineLayout(device, &_pipelineLayoutCreateInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    context.PipelineLayout = _pipelineLayout;

    // build the actual pipeline
    // we now use all of the info structs we have been writing into into this one
    // to create the pipeline
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0;
    // connect the renderInfo to the pNext extension mechanism
    // pipelineInfo.pNext = &_renderInfo;
    pipelineCreateInfo.stageCount = (uint32_t)_shaderStages.size();
    pipelineCreateInfo.pStages = _shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &_vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState = &_inputAssembly;
    pipelineCreateInfo.pTessellationState = nullptr;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &_rasterizer;
    pipelineCreateInfo.pMultisampleState = &_multisampling;
    pipelineCreateInfo.pDepthStencilState = &_depthStencil;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    pipelineCreateInfo.pDynamicState = &dynamicInfo;
    pipelineCreateInfo.layout = _pipelineLayout;
    pipelineCreateInfo.renderPass = context.RenderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    // its easy to error out on create graphics pipeline, so we handle it a bit
    // better than the common VK_CHECK case
    VkPipeline newPipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
                                  nullptr, &newPipeline) != VK_SUCCESS)
    {
        std::cout << "failed to create pipeline" << std::endl;
        return VK_NULL_HANDLE; // failed to create graphics pipeline
    }
    else
    {
        return newPipeline;
    }
}

//#pragma optimize("", on)

void PipelineBuilder::set_shaders(VkShaderModule vertexShader, VkShaderModule fragmentShader)
{
    _shaderStages.clear();

    _shaderStages.push_back(
        vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));

    _shaderStages.push_back(
        vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));
}

void PipelineBuilder::set_input_topology(VkPrimitiveTopology topology)
{
    _inputAssembly.topology = topology;
    // we are not going to use primitive restart on the entire tutorial so leave
    // it on false
    _inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void PipelineBuilder::set_raster_defaults()
{
    _rasterizer.depthClampEnable = VK_FALSE;
    _rasterizer.rasterizerDiscardEnable = VK_FALSE;
    _rasterizer.depthBiasEnable = VK_FALSE;
}

void PipelineBuilder::set_polygon_mode(VkPolygonMode mode)
{
    _rasterizer.polygonMode = mode;
    _rasterizer.lineWidth = 1.0f;
}

void PipelineBuilder::set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace)
{
    _rasterizer.cullMode = cullMode;
    _rasterizer.frontFace = frontFace;
}

void PipelineBuilder::set_multisampling_none()
{
    _multisampling.sampleShadingEnable = VK_FALSE;
    // multisampling defaulted to no multisampling (1 sample per pixel)
    _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    _multisampling.minSampleShading = 1.0f;
    _multisampling.pSampleMask = nullptr;
    // no alpha to coverage either
    _multisampling.alphaToCoverageEnable = VK_FALSE;
    _multisampling.alphaToOneEnable = VK_FALSE;
}

void PipelineBuilder::disable_blending()
{
    // default write mask
    _colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // no blending
    _colorBlendAttachment.blendEnable = VK_FALSE;
}

void PipelineBuilder::enable_blending_additive()
{
    _colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    _colorBlendAttachment.blendEnable = VK_TRUE;
    _colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    _colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    _colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    _colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
}

void PipelineBuilder::enable_blending_alphablend()
{
    _colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    _colorBlendAttachment.blendEnable = VK_TRUE;
    _colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    _colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    _colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    _colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    _colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    _colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
}

void PipelineBuilder::set_color_attachment_format(VkFormat format)
{
    _colorAttachmentformat = format;
    // connect the format to the renderInfo  structure
    _renderInfo.colorAttachmentCount = 1;
    _renderInfo.pColorAttachmentFormats = &_colorAttachmentformat;
}

void PipelineBuilder::set_depth_format(VkFormat format)
{
    _renderInfo.depthAttachmentFormat = format;
}

void PipelineBuilder::disable_depthtest()
{
    _depthStencil.depthTestEnable = VK_FALSE;
    _depthStencil.depthWriteEnable = VK_FALSE;
    _depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
    _depthStencil.depthBoundsTestEnable = VK_FALSE;
    _depthStencil.stencilTestEnable = VK_FALSE;
    _depthStencil.front = {};
    _depthStencil.back = {};
    _depthStencil.minDepthBounds = 0.f;
    _depthStencil.maxDepthBounds = 1.f;
}

void PipelineBuilder::enable_depthtest(bool depthWriteEnable, VkCompareOp op)
{
    _depthStencil.depthTestEnable = VK_TRUE;
    _depthStencil.depthWriteEnable = depthWriteEnable;
    _depthStencil.depthCompareOp = op;
    _depthStencil.depthBoundsTestEnable = VK_FALSE;
    _depthStencil.stencilTestEnable = VK_FALSE;
    _depthStencil.front = {};
    _depthStencil.back = {};
    _depthStencil.minDepthBounds = 0.f;
    _depthStencil.maxDepthBounds = 1.f;
}

bool vkutil::load_shader_module(const std::string &filePath,
                                VkDevice device,
                                VkShaderModule &outShaderModule)
{
    std::vector<uint32_t> buffer;

    Utils::read_file(filePath, buffer);

    //std::cout << "Loaded shader file, size: " << buffer.size() << std::endl;

    // create a new shader module, using the buffer we loaded
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;

    // codeSize has to be in bytes, so multply the ints in the buffer by size of
    // int to know the real size of the buffer
    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    // check that the creation goes well.
    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS)
    {
        std::cerr << "Failed to create shader module! VkResult: " << result << std::endl;
        return false;
    }

    outShaderModule = shaderModule;
    return true;
}
