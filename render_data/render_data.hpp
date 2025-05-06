#ifndef RENDERDATA_H
#define RENDERDATA_H

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
};

#endif