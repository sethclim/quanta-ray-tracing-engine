add_library(core 
    # src/main.cpp 
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
    src/utils/Utilities.hpp
    src/utils/Utilities.cpp
    src/SceneManager.hpp
    src/SceneManager.cpp
)

target_link_libraries(core PRIVATE quanta)
target_link_libraries(core PRIVATE glfw)
# target_link_libraries(core PUBLIC editor)
target_link_libraries(core PRIVATE glm::glm-header-only)
target_link_libraries(quanta PRIVATE vmath)
# target_link_libraries(core PRIVATE imgui)

target_link_libraries(core PRIVATE Vulkan::Vulkan)
target_link_libraries(core PRIVATE stb_image)
target_link_libraries(core PRIVATE pugixml)

target_include_directories(core PRIVATE ${QUANTA_INCLUDE_DIR})

target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")