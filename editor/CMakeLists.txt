
set(MapleUI_DIR C:/Users/sethc/coding/cplusplus/graphics/MapleUI/_install/lib/cmake/MapleUI)
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
get_target_property(TARGET_TYPE MapleUI::mapleui TYPE)
message(STATUS "MapleUI::MapleUI Target Type: ${TARGET_TYPE}")
            
target_include_directories(editor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
# target_include_directories(editor PUBLIC ${MapleUI_INCLUDE_DIRS})
target_link_libraries(editor PRIVATE  MapleUI::mapleui)