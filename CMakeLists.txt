cmake_minimum_required(VERSION 3.29)

project(QuantaRayTracingEngine VERSION 0.0.1)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(DEPENDENCIES CACHE STRING "")

message("dep ${DEPENDENCIES}")

add_subdirectory("${DEPENDENCIES}/glfw" glfw)
add_subdirectory("${DEPENDENCIES}/glm" glm)

add_subdirectory(quanta)
add_subdirectory(editor)
add_subdirectory(app)
add_subdirectory(vendor)

target_compile_features(app PRIVATE cxx_std_17)
target_compile_features(quanta PRIVATE cxx_std_17)
target_compile_features(editor PRIVATE cxx_std_17)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app)
