
set(MapleUI_DIR CACHE STRING C:/Users/sethc/coding/cplusplus/graphics/MapleUI/_install/lib/cmake/MapleUI)
find_package(MapleUI REQUIRED)

add_library(editor
            editor.cpp
            editor.hpp)

# include(ExternalProject)

# ExternalProject_Add(
#     MapleUI
#     PREFIX ${CMAKE_BINARY_DIR}/MapleUI
#     SOURCE_DIR C:/Users/sethc/coding/cplusplus/graphics/MapleUI
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
#     BUILD_COMMAND make
#     INSTALL_COMMAND make install
# )

# include(GNUInstallDirs)


# set(MapleUI_DIR C:/Users/sethc/coding/cplusplus/graphics/MapleUI/_install/lib/cmake/MapleUI)
# # find_package (MapleUI PATHS C:/Users/sethc/coding/cplusplus/graphics/MapleUI/cmake/config)
# find_package(MapleUI REQUIRED)




# Ensure your target depends on the external library
# add_dependencies(editor MapleUI)docker

# Link your target with the external library
# target_link_libraries(editor ${CMAKE_BINARY_DIR}/external_lib/lib/MapleUI.a)

        
# include(FetchContent)

# FetchContent_Declare(
#     MapleUI
#     SOURCE_DIR C:/Users/sethc/coding/cplusplus/graphics/MapleUI # Path to the local library
# )

# FetchContent_MakeAvailable(MapleUI)

# Link it to your target
# target_link_libraries(editor MapleUI)

# Check available MapleUI targets
# get_target_property(TARGET_TYPE MapleUI::mapleui TYPE)
# message(STATUS "MapleUI::MapleUI Target Type: ${TARGET_TYPE}")
            
# target_include_directories(editor PUBLIC ${MapleUI_INCLUDE_DIRS})
target_link_libraries(editor PRIVATE  MapleUI::mapleui)

target_link_libraries(editor PRIVATE  app)
target_link_libraries(editor PRIVATE glm::glm-header-only)

find_package(Vulkan REQUIRED)
target_link_libraries(editor PRIVATE Vulkan::Vulkan)

target_link_libraries(editor PRIVATE glfw)

target_link_libraries(editor PRIVATE RenderData)

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