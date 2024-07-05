add_executable(app 
    src/main.cpp 
    src/Application.cpp   
    src/Application.hpp
    src/WindowController.cpp   
    src/WindowController.hpp
    src/Singleton.hpp
    src/Resolution.hpp
    src/StandardIncludes.hpp
    src/VulkanBackend.hpp
    src/VulkanBackend.cpp
    src/Random.hpp
    src/Random.cpp
)

target_link_libraries(app PRIVATE quanta)
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PRIVATE glm::glm-header-only)

find_package(Vulkan REQUIRED)
target_link_libraries(app PRIVATE Vulkan::Vulkan)

target_link_libraries(app PRIVATE stb_image)