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
    src/VK_Types.cpp
    src/VK_Initializers.cpp
    src/VK_Initializers.hpp
    src/ImGUI.hpp
    src/ImGUI.cpp
    src/utils/Utilities.hpp
    src/utils/Utilities.cpp
    src/SceneManager.hpp
    src/SceneManager.cpp
)

target_link_libraries(app PRIVATE quanta)
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PUBLIC editor)
target_link_libraries(app PRIVATE glm::glm-header-only)
target_link_libraries(app PRIVATE imgui)

target_include_directories(app PRIVATE ${QUANTA_INCLUDE_DIR})


target_link_libraries(app PRIVATE Vulkan::Vulkan)
target_link_libraries(app PRIVATE stb_image)
target_link_libraries(app PRIVATE pugixml)