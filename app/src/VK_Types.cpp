#include "VK_Types.hpp"

namespace RenderData
{

    VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

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

        //std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
        //    {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, pos)},
        //    {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)},
        //    {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord)}
        //};

        return attributeDescriptions;
    }

    VkVertexInputBindingDescription getDebugBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(RenderData::DebugVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }
    std::array<VkVertexInputAttributeDescription, 1> getDebugAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

        // Position (location = 0)
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(RenderData::DebugVertex, position);

      /*  std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
          {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(DebugVertex, position)},
        };*/

        return attributeDescriptions;
    }
}
