#include "Application.hpp"

// static ImGui_ImplVulkanH_Window g_MainWindowData;
static int g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;

// Per-frame-in-flight
static std::vector<std::vector<VkCommandBuffer>> s_AllocatedCommandBuffers;
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
	// Setup GLFW window
	glfwSetErrorCallback(glfw_error_callback);

	WindowController::GetInstance().NewWindow();

	Input::InputManager::Init();

	glfwSetCursorPosCallback(WindowController::GetInstance().GetWindow(), Input::cursor_position_callback);

	editor = std::make_unique<Editor>();
	glm::vec2 size = WindowController::GetInstance().GetSize();
	editor->CalculateLayout(size.x, size.y);

	DrawData drawData = editor->RenderEditor();
	{
		Scene::SceneGraph scene;

		Materials::Lambertian mat;
		mat.Color = Math::Vector3<float>(1.0, 0, 0);
		mat.EmissionColor = Math::Vector3<float>(0, 0, 0);
		mat.EmissionStrength = 0.0f;

		Materials::Lambertian mat2;
		mat2.Color = Math::Vector3<float>(1, 1, 0);
		mat2.EmissionColor = Math::Vector3<float>(1, 1, 1);
		mat2.EmissionStrength = 0.0f;

		Materials::Metal mat3 = Materials::Metal(Math::Vector3<float>(1, 1, 1));
		/*mat3.Color = Math::Vector3<float>(1, 1, 1);
		mat3.EmissionColor = Math::Vector3<float>(1, 1, 1);
		mat3.EmissionStrength = 0.0f;*/

		Materials::Material lightMaterial;
		lightMaterial.Color = Math::Vector3<float>(1, 1, 1);
		lightMaterial.EmissionColor = Math::Vector3<float>(1, 1, 1);
		lightMaterial.EmissionStrength = 0.6f;

		auto mat_one = std::make_shared<Materials::Lambertian>(mat);
		auto mat_two = std::make_shared<Materials::Lambertian>(mat2);
		auto mat_three = std::make_shared<Materials::Metal>(mat3);
		auto mat_light = std::make_shared<Materials::Material>(lightMaterial);

		Scene::Shapes::Sphere sphere;
		sphere.Origin = Math::Vector3<float>(0, 0, 0);
		sphere.Material = mat_one;
		sphere.id = 0;

		Scene::Shapes::Sphere sphere2;
		sphere2.Origin = Math::Vector3<float>(1, 1, 0);
		sphere2.Material = mat_two;
		sphere2.id = 1;

		Scene::Shapes::Sphere sphere3;
		sphere3.Origin = Math::Vector3<float>(1, -2, 1);
		sphere3.Material = mat_three;
		sphere3.Radius = 1.8f;
		sphere3.id = 2;  

		Scene::Shapes::Sphere sphere4;
		sphere4.Origin = Math::Vector3<float>(-1, 1.7, 1);
		sphere4.Material = mat_light;
		sphere4.Radius = 1.0f;
		sphere4.id = 2222;

		scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere));
		scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere2));
		scene.ray_targets.push_back(std::make_shared<Scene::Shapes::Sphere>(sphere));
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

	VulkanBackend::GetInstance().SetupVulkan(extensions, extensions_count, drawData.vertices, drawData.indices, debugBufferSize, false);

	/*ImGui_ImplVulkanH_Window *wd = &g_MainWindowData;*/
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

	while (glfwWindowShouldClose(WindowController::GetInstance().GetWindow()) == 0 && m_Running)
	{
		// PROCESS INPUT
		double newTime = glfwGetTime();
		double frameTime = newTime - m_LastFrameTime;
		m_LastFrameTime = newTime;

		float deltaTime = std::min(frameTime, fpsLimit);

		/* m_gameController->ProcessInput(deltaTime);*/
		// m_gameController->Update(deltaTime);

		// Utilities::FPSCounter::CalculateFrameRate();

		// RENDER
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*      m_gameController->Render();*/

		// glfwSwapBuffers(WindowController::GetInstance().GetWindow());

		glfwPollEvents();
		std::vector<int> dimensions = editor->CalculateLayout(
			VulkanBackend::GetInstance().GetRenderContext().Width,
			VulkanBackend::GetInstance().GetRenderContext().Height);

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
			memset(m_AccumulationData, 0, m_Image->GetWidth() * m_Image->GetHeight() * sizeof(glm::vec4));

		for (uint32_t y = 0; y < dimensions[1]; y++)
		{
			for (uint32_t x = 0; x < dimensions[0]; x++)
			{

				float normalizedX = (float)x / (float)dimensions[0];
				float normalizedY = (float)y / (float)dimensions[1];

				uint32_t idx = x + (y * dimensions[0]);

				Math::Vector3<float> color = Math::Vector3<float>(0, 0, 0);

				if (x == 535 && y == 318)
					color = renderer->PerPixel(normalizedX, normalizedY);

				m_AccumulationData[x + y * m_Image->GetWidth()] += glm::vec4(color.x, color.y, color.z, 1.0f);

				glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_Image->GetWidth()];
				accumulatedColor /= (float)m_FrameIndex;

				accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
				m_ImageData[idx] = Utils::ConvertToRGBA(accumulatedColor);
			}
		}

		m_Image->SetData(m_ImageData);


		auto d_lines = renderer->GetDebugLines();
		if (false)
		{
			vulkanBackend.updateDebugBuffer(d_lines);
		}

		glm::vec2 size = WindowController::GetInstance().GetSize();
		editor->CalculateLayout(size.x, size.y);
		DrawData drawData = editor->RenderEditor();
		VulkanBackend::GetInstance().drawFrame(drawData.indices, d_lines.size());

		m_FrameIndex++;
	}

	// m_gameController->CleanUp();
}

void Application::Close()
{
	m_Running = false;
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
