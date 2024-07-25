add_library(quanta
            quanta.cpp
            quanta.hpp
            vector3.hpp)
            
target_include_directories(quanta PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")