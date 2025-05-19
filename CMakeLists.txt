cmake_minimum_required(VERSION 3.12...3.29)

project(QuantaRayTracingEngine VERSION 0.0.1)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(DEPENDENCIES CACHE STRING "")

add_subdirectory("${DEPENDENCIES}/glfw" glfw)
add_subdirectory("${DEPENDENCIES}/glm" glm)

find_package(Vulkan REQUIRED)

add_subdirectory(vendor)
add_subdirectory(render_data)
add_subdirectory(quanta)
add_subdirectory(editor)
add_subdirectory(runtime)
add_subdirectory(core)

target_compile_features(core PRIVATE cxx_std_17)
target_compile_features(quanta PRIVATE cxx_std_17)
target_compile_features(editor PRIVATE cxx_std_17)
target_compile_features(runtime PRIVATE cxx_std_17)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)
