add_library(RenderData INTERFACE)

            # Ensure C++ is recognized
set_target_properties(RenderData PROPERTIES LINKER_LANGUAGE CXX)

# # Make sure RenderData is visible to other projects
target_include_directories(RenderData INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RenderData INTERFACE glm::glm-header-only)