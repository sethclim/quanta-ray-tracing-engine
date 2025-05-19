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

	Input::InputManager::Init();


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

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

	glfwSetCursorPosCallback(glfwWindow, Input::cursor_position_callback);
	glfwSetMouseButtonCallback(glfwWindow, Input::mouse_button_callback);

	Input::InputManager::GetInstance().AddCallback([this](const Input::MouseEvent &e)
												   {
		debug_trace_coord = glm::vec2(e.x, e.y);
		drawn = false; });

	items[0] = "Pixel Debug";
	items[1] = "All";
	items[2] = "None";

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

	drawn = false;
	useRaytracer = false;
	item_current = 2;
	samples_per_pixel = 6;
	max_bounces = 3;
	accumulate = false;

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

	Input::InputManager::GetInstance().ProcessEvents();

	// if (!drawn)
	//{
	if (!m_Image || width != m_Image->GetWidth() || height != m_Image->GetHeight())
	{
		std::cout << "[dimensions x: " << width << " y: " << height << std::endl;

		m_Image = std::make_shared<Image>(width, height, ImageFormat::RGBA, vulkanBackend);
		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
		delete[] m_AccumulationData;
		m_AccumulationData = new glm::vec4[width * height];

		renderer->UpdateImageDimensions(width, height);
	}

	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, static_cast<size_t>(m_Image->GetWidth()) * static_cast<size_t>(m_Image->GetHeight()) * sizeof(glm::vec4));

	glm::vec2 mouse = Input::InputManager::GetInstance().GetMousePosition();

	// std::thread::hardwareconcurrency();

	renderer->SetRenderSettings(samples_per_pixel, max_bounces, accumulate);

	if (useRaytracer)
	{
		renderer->Render(reinterpret_cast<float *>(m_AccumulationData), m_ImageData, m_FrameIndex);
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
	// editor->CalculateLayout(size.x, size.y);
	/*DrawData drawData = editor->RenderEditor();*/

	// imgui new frame
	ImGui_ImplVulkan_NewFrame();
	// ImGui_ImplSDL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//----------------------------------------------------------------------------
	// ImGui UI Instructions
	ImGUI::Panel::Begin();

	ImGui::SeparatorText("RENDER SETTINGS");
	ImGui::DragInt("Samples per Pixel 0..50", &samples_per_pixel, 1, 1, 50, "%d%", ImGuiSliderFlags_AlwaysClamp);
	ImGui::DragInt("Max Bounces 0..30", &max_bounces, 1, 1, 30, "%d%", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Checkbox("Accumulate", &accumulate);

	ImGui::SeparatorText("DEBUG SETTINGS");
	bool changed = false;

	ImGui::Combo("Debug Mode", &item_current, items, IM_ARRAYSIZE(items));

	ImGui::SeparatorText("SCENE SETTINGS");

	ImGui::Text("Objects");
	for (size_t i = 0; i < scene.ray_targets.size(); ++i)
	{
		ImGui::PushID(i);
		std::shared_ptr<Scene::Shapes::RayTarget> val = scene.ray_targets[i];
		auto sphere = std::dynamic_pointer_cast<Scene::Shapes::Sphere>(val);
		if (sphere)
		{
			ImGui::Text("Sphere \n");
			float *_origin_ptr = &sphere.get()->Origin.x;
			ImGui::DragFloat3("Origin", _origin_ptr, 0.1f);
			ImGui::DragFloat("Radius", &sphere.get()->Radius, 1.0f, 0.01f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

			if (ImGui::BeginCombo("Material", sphere.get()->Material ? sphere.get()->Material->GetName().c_str() : "None"))
			{
				for (size_t i = 0; i < scene.materials.size(); ++i)
				{
					bool isSelected = (scene.materials[i] == sphere.get()->Material);
					if (ImGui::Selectable(scene.materials[i]->GetName().c_str(), isSelected))
					{
						sphere.get()->Material = scene.materials[i];
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		ImGui::PopID();
	}

	ImGui::Text("Materials");

	for (size_t i = 0; i < scene.materials.size(); i++)
	{
		ImGui::PushID(i);

		std::shared_ptr<Materials::Material> material = scene.materials[i];

		ImGui::Text(material.get()->GetName().c_str());

		float *color_ptr = &material.get()->Albedo.x;
		ImGui::ColorEdit3("Color", color_ptr);

		auto emissive = std::dynamic_pointer_cast<Materials::Emissive>(material);

		if (emissive)
		{
			ImGui::DragFloat("Emission Strength", &emissive.get()->EmissionStrength, 0.05f, 0.0f, 1.0f);
			float *emission_color_ptr = &emissive.get()->EmissionColor.x;
			ImGui::ColorEdit3("Emission Color", emission_color_ptr);
		}
		else
		{

			auto metal = std::dynamic_pointer_cast<Materials::Metal>(material);
			if (metal)
				ImGui::DragFloat("Fuzz Amt", &metal.get()->fuzz, 0.05f, 0.0f, 1.0f);
		}

		// ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
		// ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);

		ImGui::Separator();
		ImGui::PopID();
	}

	ImGui::Text("Last Render: %.3fms", m_LastRenderTime);

	changed |= ImGui::Checkbox("Trace rays", &useRaytracer);

	if (ImGui::Button("Save scene"))
		sceneManager.SaveScene(scene);

	ImGUI::Panel::End();

	//----------------------------------------------------------------------------

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