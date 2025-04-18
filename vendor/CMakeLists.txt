# add_library(stb_image STATIC stb_image/stb_image.h)
# set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)   
# target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
# target_compile_definitions(stb_image PUBLIC STB_IMAGE_IMPLEMENTATION)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

set(IMGUI_SOURCES 
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp

    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

set(IMGUI_HEADERS 
    ${IMGUI_SOURCE_DIR}/imgui.h
    ${IMGUI_SOURCE_DIR}/imconfig.h
    ${IMGUI_SOURCE_DIR}/imgui_internal.h
    ${IMGUI_SOURCE_DIR}/imstb_rectpack.h
    ${IMGUI_SOURCE_DIR}/imstb_textedit.h
    ${IMGUI_SOURCE_DIR}/imstb_truetype.h
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_vulkan.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# Link with Vulkan and GLFW
target_link_libraries(imgui PRIVATE Vulkan::Vulkan glfw)

target_include_directories(imgui PUBLIC
    ${IMGUI_SOURCE_DIR}/
    ${IMGUI_SOURCE_DIR}/backends)

# get_target_property(IMGUI_INCLUDES imgui INCLUDE_DIRECTORIES)
# message(STATUS "IMGUI include dirs: ${IMGUI_INCLUDES}")

# get_target_property(IMGUI_LINK_LIBS imgui INTERFACE_LINK_LIBRARIES)
# message(STATUS "IMGUI links: ${IMGUI_LINK_LIBS}")`