
# set(MapleUI_DIR C:/Users/sethc/coding/cplusplus/graphics/MapleUI/_install/lib/cmake/MapleUI)
# find_package(MapleUI REQUIRED)

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
    src/Image.hpp
    src/Image.cpp
    src/InputManager.hpp
    src/InputManager.cpp
    src/VK_Pipeline.hpp
    src/VK_Pipeline.cpp
    src/VK_Types.hpp
)

target_link_libraries(app PRIVATE quanta)
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PUBLIC editor)
target_link_libraries(app PRIVATE glm::glm-header-only)

find_package(Vulkan REQUIRED)

target_link_libraries(app PRIVATE Vulkan::Vulkan)

target_link_libraries(app PRIVATE stb_image)
