#include "Application.hpp"

// static ImGui_ImplVulkanH_Window g_MainWindowData;
// static int g_MinImageCount = 2;
// static bool g_SwapChainRebuild = false;

// Per-frame-in-flight
// static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
static std::vector<std::vector<std::function<void()>>> s_ResourceFreeQueue;

static uint32_t s_CurrentFrameIndex = 0;

namespace Utils
{

	static uint32_t ConvertToRGBA(const glm::vec4 &color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

Application::Application()
{
	vulkanBackend = VulkanBackend();
}

Application::~Application()
{
	Shutdown();
}

void Application::Init(DrawData drawData)
{
	debug = false;
	debug_trace_coord = glm::vec2(-1, -1);

	m_DrawData = drawData;

	// Setup GLFW window
	glfwSetErrorCallback(glfw_error_callback);

	Window::WindowController::Init(vulkanBackend);
	Window::WindowController::GetInstance().NewWindow();

	Core::Input::InputManager::Init();

	GLFWwindow *glfwWindow = Window::WindowController::GetInstance().GetWindow();
	// ImGui_ImplGlfw_InitForVulkan(glfwWindow, false);

	/*editor = std::make_unique<Editor>();*/
	glm::vec2 size = Window::WindowController::GetInstance().GetSize();
	// editor->CalculateLayout(size.x, size.y);

	// DrawData drawData = editor->RenderEditor();

	{
		// sceneManager.SaveScene(scene);
		sceneManager.LoadScene(scene, "default_scene.xml");

		renderer = std::make_unique<Renderer>(scene);
	}

	// Setup Vulkan
	if (!glfwVulkanSupported())
	{
		std::cerr << "GLFW: Vulkan not supported!\n";
		return;
	}

	uint32_t extensions_count = 0;
	const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

	auto &renderContext = vulkanBackend.GetRenderContext();

	glfwGetFramebufferSize(
		Window::WindowController::GetInstance().GetWindow(),
		&renderContext.Width,
		&renderContext.Height);

	VkDeviceSize debugBufferSize = sizeof(Utilities::DebugLine) * 1572864;

	vulkanBackend.SetupVulkan(extensions, extensions_count, drawData.vertices, drawData.indices, debugBufferSize, debug);

	// glfwSetCursorPosCallback(glfwWindow, Core::Input::cursor_position_callback);
	// glfwSetMouseButtonCallback(glfwWindow, Core::Input::mouse_button_callback);

	// Input::InputManager::GetInstance().AddCallback([this](const Input::MouseEvent &e)
	// 											   {
	// 	debug_trace_coord = glm::vec2(e.x, e.y);
	// 	drawn = false; });

	items[0] = "Pixel Debug";
	items[1] = "All";
	items[2] = "None";

	// vulkanBackend.init_imgui();

	// ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;
	// SetupVulkanWindow(wd, surface, w, h);

	// s_AllocatedCommandBuffers.resize(wd->ImageCount);
	// s_ResourceFreeQueue.resize(wd->ImageCount);
}

void Application::Run()
{
	double t = 0.0;

	const double fpsLimit = 1.0 / 60.0;

	m_Running = true;

	while (glfwWindowShouldClose(Window::WindowController::GetInstance().GetWindow()) == 0 && m_Running)
	{
		RenderLoop(t, fpsLimit);
	}

	//// m_gameController->CleanUp();
}

void Application::RenderLoop(double t, double fpsLimit)
{
	m_Timer.Reset();

	// PROCESS INPUT
	double newTime = glfwGetTime();
	double frameTime = newTime - m_LastFrameTime;
	m_LastFrameTime = newTime;

	float deltaTime = std::min(frameTime, fpsLimit);

	/* m_gameController->ProcessInput(deltaTime);*/
	// m_gameController->Update(deltaTime);

	// Utilities::FPSCounter::CalculateFrameRate();
	// Utilities::FPSCounter::CalculateFrameRate();

	// RENDER
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*      m_gameController->Render();*/

	// glfwSwapBuffers(WindowController::GetInstance().GetWindow());

	glfwPollEvents();
	int width = vulkanBackend.GetRenderContext().Width;
	int height = vulkanBackend.GetRenderContext().Height;

	std::vector<Utilities::DebugLine> d_lines;

	Core::Input::InputManager::GetInstance().ProcessEvents();

	// if (!drawn)
	//{
	if (!m_Image || width != m_Image->GetWidth() || height != m_Image->GetHeight())
	{
		std::cout << "[dimensions x: " << width << " y: " << height << std::endl;

		m_Image = std::make_shared<Image>(width, height, ImageFormat::RGBA, vulkanBackend);
		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
		delete[] m_AccumulationData;
		m_AccumulationData = new Math::Vector4<float>[width * height];

		renderer->UpdateImageDimensions(width, height);
	}

	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, static_cast<size_t>(m_Image->GetWidth()) * static_cast<size_t>(m_Image->GetHeight()) * sizeof(Math::Vector4<float>));

	glm::vec2 mouse = Core::Input::InputManager::GetInstance().GetMousePosition();

	// std::thread::hardwareconcurrency();

	renderer->SetRenderSettings(samples_per_pixel, max_bounces, accumulate);

	if (useRaytracer)
	{
		renderer->Render(m_AccumulationData, m_ImageData, m_FrameIndex);
	} // useRaytracer

	// drawn = true;
	// std::cout << "image generated " << std::endl;
	// std::cout << " First pixel: " << std::hex << m_ImageData[0] << std::endl;

	m_Image->SetData(vulkanBackend, m_ImageData);

	d_lines = renderer->GetDebugLines();
	if (debug)
	{
		vulkanBackend.updateDebugBuffer(d_lines);
	}

	glm::vec2 size = Window::WindowController::GetInstance().GetSize();

	vulkanBackend.drawFrame(m_DrawData.indices, d_lines.size());

	m_FrameIndex++;

	m_LastRenderTime = m_Timer.ElapsedMillis();
	//}
}

void Application::Close()
{
	/*m_Running = false;*/
}

void Application::Shutdown()
{
	// Cleanup
	VkResult err = vkDeviceWaitIdle(vulkanBackend.GetDevice());
	check_vk_result(err);

	// Free resources in queue
	for (auto &queue : s_ResourceFreeQueue)
	{
		for (auto &func : queue)
			func();
	}

	s_ResourceFreeQueue.clear();

	// ImGui_ImplVulkan_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();

	// CleanupVulkanWindow();
	// CleanupVulkan();

	glfwDestroyWindow(Window::WindowController::GetInstance().GetWindow());
	glfwTerminate();

	m_Running = false;
}