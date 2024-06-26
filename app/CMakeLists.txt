add_executable(app 
    src/main.cpp 
    src/Application.cpp   
    src/Application.hpp
    src/WindowController.cpp   
    src/WindowController.hpp
    src/Singleton.hpp
    src/Resolution.hpp
    src/StandardIncludes.hpp
)

target_link_libraries(app PRIVATE quanta)
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PRIVATE glm::glm-header-only)