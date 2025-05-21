add_executable(editor
            main.cpp
            editor.cpp
            editor.hpp
            ImGuiHelpers.cpp
            ImGuiHelpers.hpp)

target_link_libraries(editor PRIVATE  core)
target_link_libraries(editor PRIVATE glm::glm-header-only)
target_link_libraries(editor PRIVATE vmath)

find_package(Vulkan REQUIRED)
target_link_libraries(editor PRIVATE Vulkan::Vulkan)

target_link_libraries(editor PRIVATE glfw)

target_link_libraries(editor PRIVATE RenderData)

target_link_libraries(editor PRIVATE imgui)

# added
target_link_libraries(editor PRIVATE quanta)
target_link_libraries(editor PRIVATE glfw)
# target_link_libraries(core PUBLIC editor)
target_link_libraries(editor PRIVATE glm::glm-header-only)
target_link_libraries(editor PRIVATE imgui)

target_link_libraries(editor PRIVATE Vulkan::Vulkan)
target_link_libraries(editor PRIVATE stb_image)
target_link_libraries(editor PRIVATE pugixml)
# end added ----------------------------------------

target_link_libraries(editor PRIVATE imgui)

# target_include_directories(editor PRIVATE
#     ${IMGUI_SOURCE_DIR}/
#     ${IMGUI_SOURCE_DIR}/backends)

target_include_directories(editor PUBLIC
    $<TARGET_PROPERTY:imgui,INTERFACE_INCLUDE_DIRECTORIES>
)



if (TARGET imgui)
message(STATUS "imgui target exists before linking!")
else()
message(FATAL_ERROR "imgui target does NOT exist before linking!")
endif()

# target_include_directories(editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/")

get_target_property(INCLUDE_DIRS editor INCLUDE_DIRECTORIES)
message(STATUS "Editor include dirs: ${INCLUDE_DIRS}")

get_target_property(EDITOR_LINKED editor LINK_LIBRARIES)
message(STATUS "Editor links: ${EDITOR_LINKED}")

get_target_property(IMGUI_INCLUDES imgui INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "imgui includes: ${IMGUI_INCLUDES}")

get_target_property(EDITOR_INCLUDES editor INCLUDE_DIRECTORIES)
message(STATUS "Editor include dirs: ${EDITOR_INCLUDES}")
