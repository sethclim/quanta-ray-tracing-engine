# add_library(stb_image STATIC stb_image/stb_image.h)
# set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)   
# target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
# target_compile_definitions(stb_image PUBLIC STB_IMAGE_IMPLEMENTATION)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)