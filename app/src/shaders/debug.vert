#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * vec4(inPosition, 1.0);

    // Determine start (0.0) or end (1.0) of the line using the vertex index
    float gradient = gl_VertexIndex % 2; // 0 for start vertex, 1 for end vertex

    // Define gradient colors
    vec3 colorStart = vec3(1, 0, 0); // Red
    vec3 colorEnd = vec3(0, 0, 1);   // Blue


    // Interpolate color based on whether this is the start or end of the line
    fragColor = mix(colorStart, colorEnd, gradient);
}