#ifndef STANDARDINCLUDES_H
#define STANDARDINCLUDES_H

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include <set>
#include <fstream>
#include <array>
#include <memory>
#include <chrono>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#include <vulkan/vulkan.h>

#include "../../render_data/render_data.hpp"


#define M_ASSERT(_cond, _msg) \
    if (!(_cond))             \
    {                         \
        LogError(_msg);       \
        std::abort();         \
        glfwTerminate();      \
    }

#define M_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*_ARR)))

static void Log(std::string _msg)
{
    std::cout << _msg << std::endl;
}

static void LogError(std::string _msg)
{
    std::string msg = "[ERROR] ";
    msg.append(_msg);
    Log(msg);
}

static void check_vk_result(VkResult error)
{
    if (error == 0)
        return;
    // fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    std::string msg = "[vulkan]: VkResult =  ";
    msg.append(std::to_string(error));
    LogError(msg);
    if (error < 0)
        std::abort();
}

static void glfw_error_callback(int error, const char *description)
{
    // fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    std::string msg = "[glfw] ";
    msg.append(std::to_string(error));
    msg.append(description);
    LogError(msg);
}

#endif // !STANDARDINCLUDES_H