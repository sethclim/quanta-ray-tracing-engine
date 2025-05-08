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
	Init();
}

Application::~Application()
{
	Shutdown();
}

void Application::Init()
{
	debug = false;
	debug_trace_coord = glm::vec2(-1, -1);

	// Setup GLFW window
	glfwSetErrorCallback(glfw_error_callback);

	WindowController::GetInstance().NewWindow();

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

	GLFWwindow *glfwWindow = WindowController::GetInstance().GetWindow();
	// ImGui_ImplGlfw_InitForVulkan(glfwWindow, false);

	editor = std::make_unique<Editor>();
	glm::vec2 size = WindowController::GetInstance().GetSize();
	editor->CalculateLayout(size.x, size.y);

	DrawData drawData = editor->RenderEditor();
	{

		Materials::Lambertian mat = Materials::Lambertian("Lambertian 0");
		mat.Color = Math::Vector3<float>(1, 1, 1);

		// Materials::Lambertian mat2 = Materials::Lambertian("Lambertian 1");
		// mat2.Color = Math::Vector3<float>(0, 1, 0);
		// mat2.EmissionColor = Math::Vector3<float>(0, 0, 0);
		// mat2.EmissionStrength = 0.0f;

		// Materials::Lambertian floor_mat = Materials::Lambertian("Floor");
		// floor_mat.Color = Math::Vector3<float>(1, 1, 1);
		// floor_mat.EmissionColor = Math::Vector3<float>(0, 0, 0);
		// floor_mat.EmissionStrength = 0.0f;

		// Materials::Lambertian mat3 = Materials::Lambertian("Lambertian 3");
		// mat3.Color = Math::Vector3<float>(1, 0, 0);

		Materials::Material lightMaterial = Materials::Material("Light");
		lightMaterial.Color = Math::Vector3<float>(1, 1, 1);
		lightMaterial.EmissionColor = Math::Vector3<float>(1, 1, 1);
		lightMaterial.EmissionStrength = 1.0f;

		scene.materials.push_back(std::make_shared<Materials::Lambertian>(mat));
		// scene.materials.push_back(std::make_shared<Materials::Lambertian>(mat2));
		// scene.materials.push_back(std::make_shared<Materials::Lambertian>(floor_mat));
		// scene.materials.push_back(std::make_shared<Materials::Lambertian>(mat3));
		scene.materials.push_back(std::make_shared<Materials::Material>(lightMaterial));

		Scene::Shapes::Sphere sphere;
		sphere.Origin = Math::Vector3<float>(-0.5, 0.8, -1);
		sphere.Material = scene.materials[0];
		sphere.id = 666;

		// Scene::Shapes::Sphere floor;
		// floor.Origin = Math::Vector3<float>(0, 12, -4);
		// floor.Material = scene.materials[2];
		// floor.Radius = 10.0f;
		// floor.id = 456;

		// Scene::Shapes::Sphere sphere2;
		// sphere2.Origin = Math::Vector3<float>(1, 0, 0);
		// sphere2.Material = scene.materials[1];
		// sphere2.id = 1;

		// Scene::Shapes::Sphere sphere3;
		// sphere3.Origin = Math::Vector3<float>(-0.2, 0.2, -0.3);
		// sphere3.Material = scene.materials[3];
		// sphere3.Radius = 0.6f;
		// sphere3.id = 2;

		Scene::Shapes::Sphere sphere4;
		sphere4.Origin = Math::Vector3<float>(0.5, -0.5, 0.5);
		sphere4.Material = scene.materials[1];
		sphere4.Radius = 1.3f;
		sphere4.id = 2222;

		// scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(floor));
		scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere));
		// scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere2));
		// scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere3));
		scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere4));

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

	VulkanBackend &vulkanBackend = VulkanBackend::GetInstance();
	auto &renderContext = VulkanBackend::GetInstance().GetRenderContext();

	glfwGetFramebufferSize(
		WindowController::GetInstance().GetWindow(),
		&renderContext.Width,
		&renderContext.Height);

	VkDeviceSize debugBufferSize = sizeof(Utilities::DebugLine) * 1572864;

	VulkanBackend::GetInstance().SetupVulkan(extensions, extensions_count, drawData.vertices, drawData.indices, debugBufferSize, debug);

	glfwSetCursorPosCallback(glfwWindow, Input::cursor_position_callback);
	glfwSetMouseButtonCallback(glfwWindow, Input::mouse_button_callback);

	Input::InputManager::GetInstance().AddCallback([this](const Input::MouseEvent &e)
												   {
		debug_trace_coord = glm::vec2(e.x, e.y);
		drawn = false; });

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

	VulkanBackend &vulkanBackend = VulkanBackend::GetInstance();

	drawn = false;
	bool useRaytracer = true;
	const char *items[] = {"Pixel Debug", "All", "None"};
	static int item_current = 2;
	int samples_per_pixel = 6;
	int max_bounces = 3;
	bool accumulate = true;

	while (glfwWindowShouldClose(WindowController::GetInstance().GetWindow()) == 0 && m_Running)
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
		std::vector<int> dimensions = editor->CalculateLayout(
			VulkanBackend::GetInstance().GetRenderContext().Width,
			VulkanBackend::GetInstance().GetRenderContext().Height);

		std::vector<Utilities::DebugLine> d_lines;

		Input::InputManager::GetInstance().ProcessEvents();


		// if (!drawn)
		//{
		if (!m_Image || dimensions[0] != m_Image->GetWidth() || dimensions[1] != m_Image->GetHeight())
		{
			std::cout << "[dimensions x: " << dimensions[0] << " y: " << dimensions[1] << std::endl;

			m_Image = std::make_shared<Image>(dimensions[0], dimensions[1], ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[dimensions[0] * dimensions[1]];
			delete[] m_AccumulationData;
			m_AccumulationData = new glm::vec4[dimensions[0] * dimensions[1]];
		}

		if (m_FrameIndex == 1)
			memset(m_AccumulationData, 0, static_cast<size_t>(m_Image->GetWidth()) * static_cast<size_t>(m_Image->GetHeight()) * sizeof(glm::vec4));

		glm::vec2 mouse = Input::InputManager::GetInstance().GetMousePosition();

		if (useRaytracer)
		{
			for (uint32_t y = 0; y < dimensions[1]; y++)
			{
				for (uint32_t x = 0; x < dimensions[0]; x++)
				{

					//float flipped_y = dimensions[1] - y;

					//float normalizedX = (float)x / (float)dimensions[0];
					//float normalizedY = (float)flipped_y / (float)dimensions[1];

					uint32_t idx = x + (y * dimensions[0]);
					//bool debug_pixel = false;
					//if (item_current == 0)
					//	debug_pixel = (x == debug_trace_coord.x && y == debug_trace_coord.y);
					//else if (item_current == 1)
					//	debug_pixel = idx % 1000;

					//Math::Vector3<float> color = Math::Vector3<float>(0, 0, 0);
					///*
					//if (flipped_y < dimensions[1] / 2)
					//	color = Math::Vector3<float>(1, 0, 0);*/

					///*	if (x == 535 && y == 318)*/
					//color = renderer->PerPixel(normalizedX, normalizedY, samples_per_pixel, max_bounces, debug_pixel);

					//if (accumulate)
					//	m_AccumulationData[x + y * m_Image->GetWidth()] += glm::vec4(color.x, color.y, color.z, 1.0f);
					//else
					//	m_AccumulationData[x + y * m_Image->GetWidth()] = glm::vec4(color.x, color.y, color.z, 1.0f);

					//glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_Image->GetWidth()];
					//accumulatedColor /= (float)m_FrameIndex;

					//accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));

					//// if (x < dimensions[0] / 2)
					////	accumulatedColor = glm::vec4(1, 0, 0, 1);

					m_ImageData[idx] = Utils::ConvertToRGBA(glm::vec4(1.0f, 0.0f,0.0f, 1.0f));
				}
			}
		}

		// drawn = true;
		// std::cout << "image generated " << std::endl;

		m_Image->SetData(m_ImageData);

		d_lines = renderer->GetDebugLines();
		if (debug)
		{
			vulkanBackend.updateDebugBuffer(d_lines);
		}

		glm::vec2 size = WindowController::GetInstance().GetSize();
		editor->CalculateLayout(size.x, size.y);
		DrawData drawData = editor->RenderEditor();

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
		ImGui::DragInt("Max Bounces 0..30", &max_bounces, 1, 1, 3, "%d%", ImGuiSliderFlags_AlwaysClamp);
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
				Math::Vector3<float> _origin = sphere.get()->Origin;
				auto pos = glm::vec3(_origin.x, _origin.y, _origin.z);
				ImGui::DragFloat3("Origin", glm::value_ptr(pos), 0.1f);
				ImGui::DragFloat("Radius", &sphere.get()->Radius, 1.0f, 0.01f, 100.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

				if (ImGui::BeginCombo("Material", sphere.get()->Material ? sphere.get()->Material->GetName().c_str() : "None"))
				{
					for (size_t i = 0; i < scene.materials.size(); ++i)
					{
						bool isSelected = (scene.materials[i] == sphere.get()->Material);
						if (ImGui::Selectable(scene.materials[i]->GetName().c_str(), isSelected))
						{
							sphere.get()->Material = scene.materials[i]; // Update shared_ptr
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

			float *color_ptr = &material.get()->Color.x;
			ImGui::ColorEdit3("Color", color_ptr);

			ImGui::DragFloat("Emission Strength", &material.get()->EmissionStrength, 0.05f, 0.0f, 1.0f);
			float *emission_color_ptr = &material.get()->EmissionColor.x;
			ImGui::ColorEdit3("Emission Color", emission_color_ptr);
			// ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
			// ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);

			ImGui::Separator();
			ImGui::PopID();
		}

		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);

		changed |= ImGui::Checkbox("Trace rays", &useRaytracer);
		ImGUI::Panel::End();

		//----------------------------------------------------------------------------

		VulkanBackend::GetInstance().drawFrame(drawData.indices, d_lines.size());

		m_FrameIndex++;

		m_LastRenderTime = m_Timer.ElapsedMillis();
		//}
	}

	//// m_gameController->CleanUp();
}

void Application::Close()
{
	/*m_Running = false;*/
}

void Application::Shutdown()
{
	// Cleanup
	VkResult err = vkDeviceWaitIdle(VulkanBackend::GetInstance().GetDevice());
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

	glfwDestroyWindow(WindowController::GetInstance().GetWindow());
	glfwTerminate();

	m_Running = false;
}