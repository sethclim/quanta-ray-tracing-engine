add_executable(runtime 
    main.cpp
    Runtime.cpp
    Runtime.hpp
)

target_link_libraries(runtime PRIVATE core)

find_package(Vulkan REQUIRED)
target_link_libraries(runtime PRIVATE Vulkan::Vulkan)
target_link_libraries(runtime PRIVATE glm::glm-header-only)
target_link_libraries(runtime PRIVATE quanta)
target_link_libraries(runtime PRIVATE glfw)
target_link_libraries(runtime PRIVATE pugixml)
