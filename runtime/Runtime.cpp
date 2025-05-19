#include "Runtime.hpp"

Runtime::Runtime()
{

    DrawData dd;

    // float vp_w = width * 0.2f;
    // float vp_h = height * 0.2f;

    float left_x = -1.0;
    float right_x = 1.0;
    // float right_x = 1.0;
    float top_y = 1.0;
    float bottom_y = -1.0;

    float btn_left_x = 1.0 * 0.55f;
    float btn_right_x = 0.9;
    float btn_top_y = 0.85f;
    float btn_bottom_y = 0.80f;

    const std::vector<Vertex> _vertices = {
        // Viewport
        {{left_x, bottom_y}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{right_x, bottom_y}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{right_x, top_y}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{left_x, top_y}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
    };

    dd.vertices = _vertices;
    dd.indices = indices;

    Init(dd);


    useRaytracer = true;
    samples_per_pixel = 6;
    max_bounces = 3;
    accumulate = true;
}