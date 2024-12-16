add_library(quanta
            quanta.cpp
            quanta.hpp
            vector3.hpp
            scene/Scene.hpp
            scene/Shapes.hpp
            utilities/Random.hpp)
            
target_include_directories(quanta PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")