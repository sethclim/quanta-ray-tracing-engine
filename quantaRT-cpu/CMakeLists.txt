add_library(quanta
            quanta.cpp
            quanta.hpp
            quanta_types.hpp
            scene/Scene.hpp
            scene/Shapes.hpp
            scene/Materials.hpp
            utilities/Random.hpp
            utilities/Interval.hpp
            )
            
target_include_directories(quanta PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(quanta PRIVATE vmath)