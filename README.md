# quanta-ray-tracing-engine

<img src="https://github.com/user-attachments/assets/d49e5040-cf2b-42ee-a908-df0891c432c1" alt="editor" width="50%" height="auto">

Raytracing engine written in C++ and Vulkan

### Road Map ✓

-   [x] Base vulkan application for rendering
-   [x] CPU raytracing loop
-   [x] Sphere intersection
-   [x] Custom Vector Math Header
-   [x] Lambertian and Metal Materials
-   [x] Raster debug mode that draws ray traces
-   [x] ImGUI editor integration
-   [x] SIMD optimization on Vector Math
-   [x] Multithreading CPU for render loop
-   [ ] Scene XML format and loader
-   [ ] Triange hit detection
-   [ ] GPU raytracing
-   [ ] Object Loader integration
-   [ ] Controlable camera

## Install

### Windows

generate build files
update dependecies to point to a folder with GLFW and GLM (see Dependencies)

```bash
bash build.ps1

```

compile shaders (update to point to valid glslc.exe in vulkan sdk)

```bash
bash compile.ps1

```

open Visual Studio and Build / Run project

## Dependencies

### GLFW

repo: [github.com/glfw/glfw](https://github.com/glfw/glfw.git)

### GLM

repo: [github.com/g-truc/glm](https://github.com/g-truc/glm)

### ImGui

repo: [github.com/ocornut/imgui](https://github.com/ocornut/imgui)

### Stb Image

## Renders

<img src="https://github.com/user-attachments/assets/f20ffde5-9668-4cd0-a644-08284b64cc89" alt="editor" width="30%" height="auto">
<img src="https://github.com/user-attachments/assets/37113925-4ee4-4ae2-aa66-45d0e991c627" alt="metal" width="30%" height="auto">
