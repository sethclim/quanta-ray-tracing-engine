#include "ImGUI.hpp"

namespace ImGUI
{
    //--------------------------------------------------------------------------------------------------
    //
    // If GLFW has been initialized, returns the DPI scale of the primary monitor. Otherwise, returns 1.
    //
    float getDPIScale()
    {
        // Cached DPI scale, so that this doesn't change after the first time code calls getDPIScale.
        // A negative value indicates that the value hasn't been computed yet.
        static float cached_dpi_scale = -1.0f;

        if (cached_dpi_scale < 0.0f)
        {
            // Compute the product of the monitor DPI scale and any DPI scale
            // set in the NVPRO_DPI_SCALE variable.
            cached_dpi_scale = 1.0f;

            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            assert(monitor);
            if (monitor != nullptr)
            {
                float y_scale;
                glfwGetMonitorContentScale(monitor, &cached_dpi_scale, &y_scale);
            }
            // Otherwise, GLFW isn't initialized yet, but might be in the future.
            // (Note that this code assumes all samples use GLFW.)

            // Multiply by the value of the NVPRO_DPI_SCALE environment variable.
            const char *dpi_env = getenv("NVPRO_DPI_SCALE");
            if (dpi_env)
            {
                const float parsed_dpi_env = strtof(dpi_env, nullptr);
                if (parsed_dpi_env != 0.0f)
                {
                    cached_dpi_scale *= parsed_dpi_env;
                }
            }

            cached_dpi_scale = (cached_dpi_scale > 0.0f ? cached_dpi_scale : 1.0f);
        }

        return cached_dpi_scale;
    }
}